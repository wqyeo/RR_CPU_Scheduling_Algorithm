from models.process_data import ProcessData
from models.round_robin_type import RoundRobinType

from typing import List

class AnalysisData:
    def __init__(self, rr_type: RoundRobinType, quantum_time: float, average_waiting_time: float, average_turnaround_time: float, average_response_time: float, total_time_taken: float, context_switch_count: int) -> None:
        self.rr_type = rr_type
        self.quantum_time = quantum_time
        self.average_waiting_time = average_response_time
        self.average_turnaround_time = average_turnaround_time
        self.average_response_time = average_response_time
        self.total_time_taken = total_time_taken
        self.context_switch_count = context_switch_count
        self.processes_data : List[ProcessData] = []
        pass
