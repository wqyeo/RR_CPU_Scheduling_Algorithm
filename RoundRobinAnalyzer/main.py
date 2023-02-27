from models.analysis_group import AnalysisGroup
from models.analysis_data import AnalysisData
from data_reader import read_from_file

import matplotlib as plt
from os import listdir
from os.path import isfile, join

DATA_FILE_DIR = "data_files"

all_files = [f for f in listdir(DATA_FILE_DIR) if isfile(join(DATA_FILE_DIR, f))]
data_files: list[str] = []
for file in all_files:
    if file.endswith(".dat"):
        data_files.append(file)

analysis_datas: list[AnalysisData] = []

for data_file in data_files:
    try:
        analysis_datas.append(read_from_file(DATA_FILE_DIR + "/" + data_file))
    except Exception as e:
        print("Failed to read from data file ("  + data_file + "), error message:")
        print(e)

analysis_groupings : dict[str,AnalysisGroup] = {}

for data in analysis_datas:
    if data.grouping in analysis_groupings:
        analysis_groupings[data.grouping].data_group.append(data)
    else:
        new_grouping = AnalysisGroup()
        new_grouping.data_group.append(data)
        analysis_groupings[data.grouping] = new_grouping
