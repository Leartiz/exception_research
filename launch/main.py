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
    def table_header() -> list:
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

basic_params = Params([5, "sequential", 1, 1_000_000, 0.0, "us"])
launches = [
    Params([basic_params.run_count, "sequential", 1, basic_params.equation_count, 0.0, basic_params.time_unit]),
    # Params([basic_params.run_count, "sequential", 1, basic_params.equation_count, 1.0, basic_params.time_unit]),
    # Params([basic_params.run_count, "sequential", 1, basic_params.equation_count, 3.0, basic_params.time_unit]),
    # Params([basic_params.run_count, "sequential", 1, basic_params.equation_count, 5.0, basic_params.time_unit]),
    # Params([basic_params.run_count, "sequential", 1, basic_params.equation_count, 10.0, basic_params.time_unit]),
    # Params([basic_params.run_count, "sequential", 1, basic_params.equation_count, 25.0, basic_params.time_unit]),
    #...
]

# ------------------------------------------------------------------------

result_table_content = [
    Params.table_header()
]

counter = 0
for one_launch in launches:
    app = ".\\launch\\mingw_2024\\app.exe"
    app += " --run_count=" + str(one_launch.run_count)
    app += " --run_type=" + one_launch.run_type
    app += " --max_thread_count=" + str(one_launch.max_thread_count)
    app += " --equation_count=" + str(one_launch.equation_count)
    app += " --error_percentage=" + str(one_launch.error_percentage)
    app += " --time_unit=" + one_launch.time_unit

    # TODO: how it works?
    import subprocess
    PIPE = subprocess.PIPE
    p = subprocess.Popen(app, shell=True, stdin=PIPE, stdout=PIPE,
                         stderr=subprocess.STDOUT, close_fds=True) 
    p.wait()

    # ***

    result_table_body_line = []
    result_table_body_line.append(counter)
    result_table_body_line.append(one_launch.run_count)
    result_table_body_line.append(one_launch.run_type)
    result_table_body_line.append(one_launch.max_thread_count)
    result_table_body_line.append(one_launch.equation_count)
    result_table_body_line.append(one_launch.error_percentage)
    
    all_lines = p.stdout.readlines()
    for line in all_lines:
        decoded_line = line.decode("cp866")
        if decoded_line.startswith("exception_count"):
            result_table_body_line.append(decoded_line.split(" ")[1].strip())
        if decoded_line.startswith("time_unit"):
            one_launch.time_unit = decoded_line.split(" ")[1].strip()
                
    import copy
    for line in all_lines:  
        decoded_line = line.decode("cp866") 
        copy_result_table_body_line = copy.deepcopy(result_table_body_line)
        
        if decoded_line.startswith("no_exception") or \
            decoded_line.startswith("normal") or \
            decoded_line.startswith("full_exception"):
                          
            copy_result_table_body_line.append(decoded_line.split(" ")[0][:-1].strip())
            copy_result_table_body_line.append(decoded_line.split(" ")[1].strip())
            copy_result_table_body_line.append(one_launch.time_unit)
            
            result_table_content.append(
                copy_result_table_body_line
            )

    counter += 1

# ------------------------------------------------------------------------

for table_line in result_table_content:    
    print(table_line)
    
csv_file = open("result.csv", "w")
for table_line in result_table_content:    
    for one_cell in table_line:
        csv_file.write(str(one_cell) + "; ")
    csv_file.write("\n")
csv_file.flush()
csv_file.close()


