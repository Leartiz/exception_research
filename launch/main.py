class Params:
    def __init__(self, list) -> None:
        self.run_count = list[0]
        self.equation_count = list[1]
        self.exception_count = list[2]
        self.type = list[3]
        pass

# edit this
# ------------------------------------------------------------------------



launches = [
    Params([5, 1_000, 0, "sequential"])
    #...
]

# ------------------------------------------------------------------------

for one_params in launches:
    pass



