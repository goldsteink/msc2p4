import MSPY

class PyUpdateNbbo(MSPY.StateComputation):
    def __init__(self):
        super(PyUpdateNbbo,self).__init__()

    def name(self):
        print "NAME-------------"
        MSPY.printKevin()
        return "PyUpdateNbbo"

    def compute(self, input_, stateChangeRepo_, stateChageRepoHelper_, state_, none_):
        print "\n\nCOMPUTE\n\n"
        nbbo = MSPY.getNbboMessageFromInput(input_)
        print "NbboMessage({}x{})".format(nbbo.bid_price(), nbbo.offer_price())
        sch = MSPY.w_state_change_repository_lookup_by_name(stateChageRepoHelper_, stateChangeRepo_, "symbol data state change");
        sc =  MSPY.w_stateful_computation_get_return(stateChageRepoHelper_,MSPY.getNullPtr(),none_)
        return sc
        
    def get_number_of_state_change_builders(self):
        print "Number of change builders:0"
        return 0

    def get_state_change_builder(self, idx_):
        print "get_state_change_builder"
        return None
        

def getNbboChecker():
    print "\n\n\nInside getNbboChecker!\n\n\n"
    b = PyUpdateNbbo()
    return b

def printMe():
    MSPY.printKevin()



