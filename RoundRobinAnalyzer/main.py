import matplotlib as plt

from data_reader import read_from_file

result = read_from_file("TestModel.dat")

print(len(result.processes_data))
print(result.average_response_time)
print("====")

for process in result.processes_data:
    print(process.turnaround_time)
