#
# Market Spread PYthon (MSPY)
#



#
# import the C++ artifacts (classes & functions)
#
import MSPY



#
#
#
debug = True
showRejectWhenTrue = True


class PySymbolDataStateChangeBulder(MSPY.StateChangeBuilder):
    def __init__(self):
        super(PySymbolDataStateChangeBuilder,self).__init__()
        return;
        
    def build(self,idx_):
        print "Within temptation!"
        return PySymbolDataStateChange(idx_)

 

class PyUpdateNbboNoOutput(MSPY.StateComputation):
    def __init__(self):
        print "------------------>"
        super(PyUpdateNbboNoChecker, self).__init__()
        
    def name(self):
        return "Update NBBO, no output"
        
    def get_number_of_state_change_builders(self):
        return 1
            
    def get_state_change_builder(self,idx_):
        print ":::>building PySymbolDataStateChangeBulder()"
        return PySymbolDataStateChangeBulder()

    def compute(self, input_, state_change_repository_, state_change_respository_helper_, state_, none_):
        print "HWRE"
        return None
        


#
# extends the MSPY.StateChange - this is a virtual
# class defined in the WallarooCppApi
#
class PySymbolDataStateChange(MSPY.StateChange):
    _shouldRejectTrades = False
    _lastBid = 0.0
    _lasOffer = 0.0

    def __init__(self):
        super(PySymbolDataStateChange,self).__init__()
        self._shouldRejectTrades = False
        self._lastBid = 0.07
        self._lastOffer = 0.0

        
    def name(self):
        return "symbol data state change";

    def apply(self, state_):
        #sd=MSPY.getSymbolData(state_)
        #sd.set_should_reject(_shouldRejectTrades)
        #sd.set_last_bid(_lastBid)
        #sd.set_last_offer(_lastOffer)
        print "--apply!"

    def update(self, shouldRejectTrades_, lastBid_, lastOffer_):
        self._shouldRejectTrades=shouldRejectTrades_
        self._lastBid = lastBid_
        self._lastOffer = lastOffer_

    def get_log_entry_size(self):
        return 0; 
    
    def to_log_entry(self):
        return

    def get_log_entry_size_header_size(self):
        return 0; 

    def read_log_entry_size_header(self):
        return MSPY.getNullPtr()

    def read_log_entry(self,bytes_) :
        return True; 



#
# extends the MSPY.StateComputatio - this is a virtual
# class defined in the WallarooCppApi
#
class PyUpdateNbbo(MSPY.StateComputation):
    maxSpread = 0.05

    def __init__(self):
        super(PyUpdateNbbo,self).__init__()
        

    def name(self):
        #if ( debug ): 
        #    print "PyUpdateNbbo.name(self):------NAME-----";
        return "PyUpdateNbbo"

    def compute(self, input_, stateChangeRepo_, stateChageRepoHelper_, state_, none_):
        try:
            nbbo = MSPY.getNbboMessageFromInput(input_)

            if ( debug ):
                print "NbboMessage({}={}x{})".format(nbbo.get_symbol(), nbbo.bid_price(), nbbo.offer_price())

            #
            # actual logic
            #
            bidAskSpread = nbbo.offer_price()-nbbo.bid_price()
            if( debug ):
                print "{} diff:{}, maxDiff:{}".format(nbbo.get_symbol(), bidAskSpread, self.maxSpread)

            shouldRejectTrades = False;
            if ( bidAskSpread >= self.maxSpread ):
                shouldRejectTrades = True
                if ( showRejectWhenTrue ):
                    print "{} REJECTING TRADES, Condition #1. BidAskSpread:{}, Bid:{}, Ask:{}, MaxAllowableDiff:{}".format(
                        nbbo.get_symbol(), bidAskSpread, nbbo.bif_price(), nbbo.offer_price, self.maxSpread)
                    
            if ( (bidAskSpread / nbbo.mid())>=self.maxSpread ):
                shouldRejectTrades = True
                if ( showRejectWhenTrue ):
                    print "{} REJECTING TRADES, Condition #B. BidAskSpread:{}, Bid:{}, Ask:{}, MaxAllowableDiff:{}".format(
                        nbbo.get_symbol(), bidAskSpread, nbbo.bif_price(), nbbo.offer_price, self.maxSpread)

            #
            # get my handles
            #
            sch = MSPY.w_state_change_repository_lookup_by_name(stateChageRepoHelper_, 
                                                                stateChangeRepo_, 
                                                                "symbol data state change");
            sc_pure = MSPY.w_state_change_get_state_change_object(stateChageRepoHelper_, sch)
            print sc_pure
            #sdsc = PySymbolDataStateChange(sc_pure)
            #print sc_pure.name()
            #sdsc.update(shouldRejectTrades, nbbo.bid_price(), nbbo.offer_price())
            return MSPY.w_stateful_computation_get_return(stateChageRepoHelper_, MSPY.getNullPtr(), sch)
        except (NameError,RuntimeError) as err:
            print "Error: {}".format(err)
            return MSPY.w_stateful_computation_get_return(stateChageRepoHelper_,
                                                          MSPY.getNullPtr(),
                                                          sch)

        
    def get_number_of_state_change_builders(self):
        print "Number of change builders:0"
        return 0

    def get_state_change_builder(self, idx_):
        print "get_state_change_builder"
        return None
        




#
# hooks for C++
#
def getNbboChecker(val_):
    try:
        print "::::::>Inside (A) getNbboChecker, val:{}!<::::::".format(val_)
        return PyUpdateNbbo()
    except (NameError,RuntimeError) as err:
        print "Error: {}".format(err)
    
def getSymbolDataStateChangeBuilder(val_):
    try:
        print "::::::>Inside (B) getStateChangeBuilder! val:{}<::::::".format(val_)
        idx=int(val_)
        return PySymbolDataStateChange(idx)
    except (NameError,RuntimeError) as err:
        print "Error: {}".format(err)

def updateNbboNoOutputChecker(val_):
    try:
        print "::::::>Inside (C) updateNbboNoOutputChecker! val:{}<::::::".format(val_)
        idx=int(val_)
        return PySymbolDataStateChange(idx)
    except (NameError,RuntimeError) as err:
        print "Error: {}".format(err)
        
def printMe():
    MSPY.printKevin()



