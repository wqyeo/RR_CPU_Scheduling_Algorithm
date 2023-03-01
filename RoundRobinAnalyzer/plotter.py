import matplotlib.pyplot as plt

from models.analysis_group import AnalysisGroup
from models.round_robin_type import RoundRobinType
from models.analysis_data import AnalysisData
from models.line_plot import LinePlot

def get_plot_color_by_rr_type(rr_type: RoundRobinType) -> str:
    if rr_type == RoundRobinType.BEST_QUANTUM_TIME:
        return "blue"
    if rr_type == RoundRobinType.MANHATTAN:
        return "red"
    if rr_type == RoundRobinType.ROUND_ROBIN:
        return "orange"
    return "cyan"

def plot_graph_analysis(analysis_groupings: dict[str, AnalysisGroup], graph_file_name: str) -> None:
    colored_line_plots : dict[str, LinePlot] = {}

    curr_x = 0
    for key, value in analysis_groupings.items():
        for analysis_data in value.get_data_group():
            data_color = get_plot_color_by_rr_type(analysis_data.rr_type)
            data_line_plot: LinePlot 
            if data_color in colored_line_plots:
                data_line_plot = colored_line_plots[data_color]
            else:
                data_line_plot = LinePlot(data_color)
                colored_line_plots[data_color] = data_line_plot
            # TODO: Plot for other values
            data_line_plot.add_plot(curr_x, round(analysis_data.average_waiting_time))
        curr_x += 1

    for key, value in colored_line_plots.items():
        plt.plot(value.get_x_plots(), value.get_y_plots(), value.get_color_plots())
    plt.savefig(graph_file_name)
