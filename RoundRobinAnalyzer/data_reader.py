from models.analysis_data import AnalysisData
from models.process_data import ProcessData
from models.round_robin_type import RoundRobinType

from enum import Enum
import re

class _ReadMode(Enum):
    OTHERS = 0
    READING_MAIN = 1
    READING_PROCESS = 2

def is_whitespace_or_none_or_validation(input_str: str) -> bool:
    if input_str is None or len(input_str.strip()) == 0:
        return True
    elif "<START OF FILE>" in input_str or "<END OF FILE>" in input_str:
        return True
    else:
        return False

def determine_rr_type_from_str(rr_type_str: str) -> RoundRobinType:
    if rr_type_str == "Best Time Quantum Round Robin":
        return RoundRobinType.BEST_QUANTUM_TIME
    elif rr_type_str == "Manhattan Round Robin":
        return RoundRobinType.MANHATTAN
    elif rr_type_str == "Eighty Five Percentile Round Robin":
        return RoundRobinType.EIGHTY_FIVE_PERCENT
    elif rr_type_str == "Sorted Round Robin":
        return RoundRobinType.SORTED
    return RoundRobinType.ROUND_ROBIN

def update_curr_process_with_line(line: str, process_data: ProcessData, index_counter: int) -> None:
    if index_counter == 0:
        # Name of process
        pass
    elif index_counter == 1:
        process_data.arrival_time = float(line.split('=')[1].strip())
    elif index_counter == 2:
        process_data.burst_time = float(line.split('=')[1].strip())
    elif index_counter == 3:
        process_data.waiting_time = float(line.split('=')[1].strip())
    elif index_counter == 4:
        process_data.response_time = float(line.split('=')[1].strip())
    elif index_counter == 5:
        process_data.turnaround_time = float(line.split('=')[1].strip())

def update_main_data_with_line(line: str, analysis_data: AnalysisData, index_counter: int) -> None:
    if index_counter == 0:
        rr_type_string = re.search(r'\[(.*?)\]', line).group(1)
        analysis_data.rr_type = determine_rr_type_from_str(rr_type_string)
    elif index_counter == 1:
        analysis_data.grouping = line.split('=')[1].strip()
    elif index_counter == 2:
        analysis_data.average_waiting_time = float(line.split('=')[1].strip())
    elif index_counter == 3:
        analysis_data.average_turnaround_time = float(line.split('=')[1].strip())
    elif index_counter == 4:
        analysis_data.average_response_time = float(line.split('=')[1].strip())
    elif index_counter == 5:
        analysis_data.total_time_taken = float(line.split('=')[1].strip())
    elif index_counter == 6:
        analysis_data.context_switch_count = int(line.split('=')[1].strip())
    elif index_counter == 7:
        # Total number of processes
        pass

def read_from_file(file_path: str) -> AnalysisData:
    """
    Read from the Round Robin data File.

    Args:
        file_path (str): The path to the file

    Returns:
        AnalysisData object, None if the file is invalid.
    """
    if (not is_valid_file(file_path)):
        return None

    curr_read_mode: _ReadMode = _ReadMode.READING_MAIN
    # For current mode, to determine what variable to store currently
    index_counter: int = 0;

    analysis_data: AnalysisData = AnalysisData()
    curr_process_data: ProcessData = ProcessData()
    with open(file_path, 'r') as f:
        lines = f.readlines()
        for line in lines:
            if is_whitespace_or_none_or_validation(line):
                continue

            if (curr_read_mode == _ReadMode.READING_MAIN):
                # Reading details from the Round robin result
                update_main_data_with_line(line, analysis_data, index_counter)
                index_counter += 1
                if (index_counter == 8):
                    # End of section
                    curr_read_mode = _ReadMode.READING_PROCESS
                    index_counter = 0
            elif (curr_read_mode == _ReadMode.READING_PROCESS):
                # Currently reading details of the current process
                update_curr_process_with_line(line, curr_process_data, index_counter)
                index_counter += 1
                if index_counter == 6:
                    # End of current section
                    analysis_data.processes_data.append(curr_process_data)
                    curr_process_data = ProcessData()
                    index_counter = 0
    return analysis_data

def is_valid_file(file_path: str) -> bool:
    """
    Checks if the file is a valid Round Robin data file.

    Args:
        file_path (str): The path to the file.

    Returns:
        bool: True if the file is a valid Round Robin data file, False otherwise.
    """

    first_word = "<START OF FILE>"
    last_word = "<END OF FILE>"
    with open(file_path, 'r') as file:
        lines = file.readlines()

    first_line = lines[0].strip()
    last_line = lines[-1].strip()

    if first_word in first_line and last_word in last_line:
        return True
    else:
        return False
