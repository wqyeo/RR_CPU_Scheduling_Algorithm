from models.analysis_data import AnalysisData

class AnalysisGroup:
    def __init__(self) -> None:
        self.data_group: list[AnalysisData] = []
        pass

    def get_data_group(self) -> list[AnalysisData]:
        return self.data_group
