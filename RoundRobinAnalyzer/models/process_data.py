class ProcessData:
    def __init__(self, arrival_time: float, burst_time: float, waiting_time: float, turnaround_time:float, response_time:float) -> None:
        self.arrival_time = arrival_time
        self.burst_time = burst_time
        self.waiting_time = waiting_time
        self.turnaround_time = turnaround_time
        self.response_time = response_time
        pass
