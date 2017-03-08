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
            #print sc_pure
            #sdsc = MSPY.castToSymbolDataSC(sc_pure)
            #MSPY.updateStateChange(sdsc, shouldRejectTrades, nbbo.bid_price(), nbbo.offer_price())
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
def getNbboChecker():
    print "\n\n\nInside getNbboChecker!\n\n\n"
    b = PyUpdateNbbo()
    return b

def printMe():
    MSPY.printKevin()



