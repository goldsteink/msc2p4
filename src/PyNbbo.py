#
# Market Spread PYthon (MSPY)
#



#
# import the C++ artifacts (classes & functions)
#
import MSPY




#
# dictionary translating b/w StateChange* (from C++) into PySymbolDataSC (python)
#
classes = {}




#
#
#
debug = True
showRejectWhenTrue = True







#
# extends the MSPY.StateChange - this is a virtual
# class defined in the WallarooCppApi
#
class PySymbolData(MSPY.State):
    _shouldRejectTrades = False
    _lastBid = 0.0
    _lastAsk  =0.0

    def __init__(self):
        MSPY.State.__init__(self)
        #super(PySymbolData,self).__init__()
        print "::::>>PySymbolData"
    
    def name():
        return "PySymbolData()"
        



#
# extends the MSPY.StateChange - this is a virtual
# class defined in the WallarooCppApi
#
class PySymbolDataSC(MSPY.SymbolDataStateChange):
    #def __init__(self):
    #    super(MSPY.SymbolDataStateChange, self).__init__()
    #    print "::::>{},type:{}".format("PySymbolDataSC",type(self))


    def __init__(self, idx_):
        #MSPY.SymbolDataStateChange.__init__(self)
        super(PySymbolDataSC, self).__init__()
        #self.id(idx_)
        print "::::>{}:{}".format("PySymbolDataSC", type(self))


    def name(self):
        print "it could be a mismatch in"
        return "PySumbolDataSC"

    def apply(self,state_):
        print "I don't articulate well"
        return None
        
    def to_log_entry(self,bytes_):
        print "A list for letting you know"
        
    def get_log_entry_size(self):
        print "THis is a part of something - "
        return 0

    def get_log_entry_size_header_size(self):
        print "rain - what a relief!"
        return 0

    def read_log_entry_size_header(self,bytes_):
        print "Yesterday it rained, and I heard"
        return 0

    def read_log_entry(self):
        return True

    def id(self):
        print ":::::id()=1"
        return 1

    def str(self):
        print ":::::str()"
        return "ME"

    def update(self, shouldRejectTrades_, bid_, ask_):
        print "ShouldRejectTrades:{}, Type:{}".format(shouldRejectTrades_, type(shouldRejectTrades_))
        self.set_should_reject_trades(shouldRejectTrades_)
        self.set_last_offer(ask_)
        self.set_last_bid(bid_)
        print ":::>update({},{},{})".format(
            self.get_should_reject_trades(),
            self.get_last_bid(),
            self.get_last_offer())
        return None

    def update2(self, shouldRejectTrades_, bid_, ask_):
        print "ShouldRejectTrades:{}, Type:{}".format(shouldRejectTrades_, type(shouldRejectTrades_))
        self.set_last_offer(ask_)
        return None

    def showLive(self):
        print "transalation, or time, or tune"
        print "Hash:{}".format(self.getHashAsString())
        

        

#
# extends the MSPY.StateComputatio - this is a virtual
# class defined in the WallarooCppApi
#
class PyUpdateNbboNoOutput(MSPY.StateComputation):
    def __init__(self):
        #MSPY.StateComputatio.__init__(self)
        super(PyUpdateNbboNoOutput, self).__init__()
        
    def name(self):
        return "Work is work, the pen is heavy"
        
    def get_number_of_state_change_builders(self):
        return 1
            
    def get_state_change_builder(self,idx_):
        print "::::::::::::::::::>building PySymbolDataStateChangeBuilder()++++++++++++++++++++++"
        print "I am well, tired, and you?"
        return getNullPtr()

    def compute(self, input_, state_change_repository_, state_change_respository_helper_, state_, none_):
        print "Here we are, again at the crosswire"
        return getNullPtr()
        


#
# extends the MSPY.StateComputatio - this is a virtual
# class defined in the WallarooCppApi
#
class PyUpdateNbbo(MSPY.StateComputation):
    maxSpread = 0.05

    def __init__(self):
        MSPY.StateComputation.__init__(self)
        #super(PyUpdateNbbo, self).__init__()
        if ( debug ):
            print "PyUpdateNbbo::PyUpdateNbbo()"
        

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
                        nbbo.get_symbol(), bidAskSpread, nbbo.bid_price(), nbbo.offer_price, self.maxSpread)
                    
            if ( (bidAskSpread / nbbo.mid())>=self.maxSpread ):
                shouldRejectTrades = True
                if ( showRejectWhenTrue ):
                    print "{} REJECTING TRADES, Condition #B. BidAskSpread:{}, Bid:{}, Ask:{}, MaxAllowableDiff:{}".format(
                        nbbo.get_symbol(), bidAskSpread, nbbo.bid_price(), nbbo.offer_price, self.maxSpread)

            #
            # get my handles
            #
            sch = MSPY.w_state_change_repository_lookup_by_name(stateChageRepoHelper_, 
                                                                stateChangeRepo_,                                                                 "symbol data state change");
            sc_pure = MSPY.w_state_change_get_state_change_object(stateChageRepoHelper_, sch)
            print "HAVE THE PURE: Hash:{}".format(sc_pure.getHashAsString())
            hash=MSPY.getWallarooHashValue(sc_pure)
            scsd = classes[hash];
            if ( scsd ):
                print "Lookup conversion successful, new object:{}".format(scsd)
                scsd.showLive()
                scsd.update(shouldRejectTrades, nbbo.bid_price(), nbbo.offer_price())
            #sc_pure.__class__=PySymbolDataSC
            #sc_pure.showLive()
            #if ( shouldRejectTrades ):
            #sc_pure.update2(shouldRejectTrades, nbbo.bid_price(), nbbo.offer_price())
            return MSPY.w_stateful_computation_get_return(stateChageRepoHelper_, MSPY.getNullPtr(), sch)
        except (NameError,RuntimeError,AttributeError) as err:
            print "Error: {}".format(err)
            return MSPY.w_stateful_computation_get_return(stateChageRepoHelper_,
                                                          MSPY.getNullPtr(),
                                                          sch)

        
    def get_number_of_state_change_builders(self):
        return 1

    def get_state_change_builder(self, idx_):
        try:
            print "-----{}::get_state_change_builder({})-----".format(self.name(), idx_)
            builder = MSPY.SymbolDataStateChangeBuilder()
            return builder
        except (NameError,RuntimeError) as err:
            print "Error: {}".format(err)
            return None
        



#
# hooks for C++
#
def getNbboChecker(val_):
    try:
        print "::::::>Inside (A) getNbboChecker, val:{}!<::::::".format(val_)
        a=PyUpdateNbbo()
        print a
        #b = PySymbolDataSC()
        #print b
        return a
    except (NameError,RuntimeError) as err:
        print "Error: {}".format(err)
    
def buildSymbolDataStateChange(val_):
    try:
        print "::::::>Inside (B) getStateChangeBuilder! val:{}<::::::".format(val_)
        idx=int(val_)
        sdsc = PySymbolDataSC(idx)
        hash=MSPY.getWallarooHashValue(sdsc)
        if ( hash==0 ):
            print "ERROR: Cannot stash this object!"
        else:
            classes[hash]=sdsc
            print "PyHashVal:{}".format(hash)
        return sdsc
    except (NameError,RuntimeError) as err:
        print "Error: {}".format(err)

def updateNbboNoOutputChecker(val_):
    try:
        print "::::::>Inside (C) updateNbboNoOutputChecker! val:{}<::::::".format(val_)
        idx=int(val_)
        return PySymbolDataSC(idx)
    except (NameError,RuntimeError) as err:
        print "Error: {}".format(err)
        
def printMe():
    MSPY.printKevin()



