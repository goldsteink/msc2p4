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
        MSPY.printKevin()
        return MSPY.w_stateful_computation_get_return(stateChangeReoHelper_,0,none_)


        
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



