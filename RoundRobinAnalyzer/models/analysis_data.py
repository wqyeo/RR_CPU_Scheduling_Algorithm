from models.process_data import ProcessData
from models.round_robin_type import RoundRobinType

from typing import List

class AnalysisData:
    def __init__(self) -> None:
        self.grouping: str = "" 
        self.rr_type: RoundRobinType = RoundRobinType.ROUND_ROBIN
        self.quantum_time: float = 0
        self.average_waiting_time: float = 0 
        self.average_turnaround_time: float = 0 
        self.average_response_time: float = 0
        self.total_time_taken: float = 0
        self.context_switch_count: int = 0
        self.processes_data : List[ProcessData] = []
        pass


