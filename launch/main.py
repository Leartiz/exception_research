import os
print(os.getcwd())

class Params:
    def __init__(self, list) -> None:
        self.run_count = list[0]
        self.run_type  = list[1]
        self.max_thread_count = list[2]
        
        self.equation_count   = list[3]
        self.error_percentage = list[4]
        
        self.time_unit = list[5]
        
    @staticmethod
    def header() -> list:
        return [
            # input:
            "№", 
            "Run Count", "Run Type",
            "Max Thread Count",
            "Equation Count",
            "Error Percentage", "Exception Count",
            
            # output:
            "Solver Type", # <--- NoException, Normal, Full Exception
            "Duration", "Time Unit"
        ]

# edit this
# ------------------------------------------------------------------------

basic_params = Params([5, "sequential", 1, 100_000, 0.0, "us"])
launches = [
    Params([basic_params.run_count, "sequential", 1, basic_params.equation_count, 0.0, basic_params.time_unit]),
    Params([basic_params.run_count, "sequential", 1, basic_params.equation_count, 1.0, basic_params.time_unit]),
    Params([basic_params.run_count, "sequential", 1, basic_params.equation_count, 3.0, basic_params.time_unit]),
    Params([basic_params.run_count, "sequential", 1, basic_params.equation_count, 5.0, basic_params.time_unit]),
    Params([basic_params.run_count, "sequential", 1, basic_params.equation_count, 10.0, basic_params.time_unit]),
    Params([basic_params.run_count, "sequential", 1, basic_params.equation_count, 25.0, basic_params.time_unit]),
    #...
]

# ------------------------------------------------------------------------

for one_launch in launches:
    app = ".\\launch\\app.exe"
    app += " --run_count=" + str(one_launch.run_count)
    app += " --run_type=" + one_launch.run_type
    app += " --max_thread_count=" + str(one_launch.max_thread_count)
    app += " --equation_count=" + str(one_launch.equation_count)
    app += " --error_percentage=" + str(one_launch.error_percentage)
    app += " --time_unit=" + one_launch.time_unit

    import subprocess
    PIPE = subprocess.PIPE
    p = subprocess.Popen(app, shell=True, stdin=PIPE, stdout=PIPE,
            stderr=subprocess.STDOUT, close_fds=True)
    p.wait()

    result_csv = app + ", "
    for line in p.stdout.readlines():    
        decoded_line = line.decode("cp866")
        if decoded_line.startswith("no_exception"):
            result_csv += decoded_line

    print(result_csv)


