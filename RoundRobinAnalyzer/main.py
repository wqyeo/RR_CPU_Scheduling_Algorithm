from models.analysis_group import AnalysisGroup
from models.analysis_data import AnalysisData
from data_reader import read_from_file
from plotter import plot_graph_analysis, GraphType 
from models.round_robin_type import RoundRobinType

import matplotlib as plt
from os import listdir
from os.path import isfile, join

DATA_FILE_DIR = "data_files"

all_files = [f for f in listdir(DATA_FILE_DIR) if isfile(join(DATA_FILE_DIR, f))]
data_files: list[str] = []
for file in all_files:
    if file.endswith(".dat"):
        data_files.append(file)

print(len(data_files))
analysis_datas: list[AnalysisData] = []

for data_file in data_files:
    try:
        analysis_datas.append(read_from_file(DATA_FILE_DIR + "/" + data_file))
    except Exception as e:
        print("Failed to read from data file ("  + data_file + "), error message:")
        print(e)

print(len(analysis_datas))
analysis_groupings : dict[str,AnalysisGroup] = {}

for data in analysis_datas:
    if data.grouping in analysis_groupings:
        analysis_groupings[data.grouping].data_group.append(data)
    else:
        new_grouping = AnalysisGroup()
        new_grouping.data_group.append(data)
        analysis_groupings[data.grouping] = new_grouping
print(len(analysis_groupings))

sorted_groupings = sorted(analysis_groupings.items(), key=lambda x: [d.total_time_taken for d in x[1].data_group if d.rr_type == RoundRobinType.ROUND_ROBIN][0])
dict_sorted_groupings = {k: v for k, v in sorted(sorted_groupings, key=lambda x: [d.total_time_taken for d in x[1].data_group if d.rr_type == RoundRobinType.ROUND_ROBIN][0])}

plot_graph_analysis(dict_sorted_groupings, "context_switch.png", "Context Switch Count", GraphType.CONTEXT_SWITCH)

plot_graph_analysis(dict_sorted_groupings, "turnaround_time.png", "Average Turnaround Time", GraphType.TURNAROUND_TIME)
plot_graph_analysis(dict_sorted_groupings, "waiting_time.png", "Average Waiting Time", GraphType.WAITING_TIME)
plot_graph_analysis(dict_sorted_groupings, "response_time.png", "Average Response Time", GraphType.RESPONSE_TIME)
