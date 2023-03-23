import matplotlib.pyplot as plt
import numpy as np
from models.analysis_group import AnalysisGroup
from models.round_robin_type import RoundRobinType
from models.analysis_data import AnalysisData
from models.line_plot import LinePlot
from scipy.interpolate import make_interp_spline
from enum import Enum
class GraphType(Enum):
    CONTEXT_SWITCH = 0,
    WAITING_TIME = 1,
    TURNAROUND_TIME = 2,
    RESPONSE_TIME = 3

def get_plot_color_by_rr_type(rr_type: RoundRobinType) -> str:
    if rr_type == RoundRobinType.BEST_QUANTUM_TIME:
        return "b"
    if rr_type == RoundRobinType.MANHATTAN:
        return "r"
    if rr_type == RoundRobinType.ROUND_ROBIN:
        return "g"
    if rr_type == RoundRobinType.SORTED:
        return "k"
    if rr_type == RoundRobinType.EIGHTY_FIVE_PERCENT:
        return "m"
    return "c"

def get_legend_name_by_rr_type(rr_type: RoundRobinType) -> str:
    if rr_type == RoundRobinType.BEST_QUANTUM_TIME:
        return "Best Time Quantum"
    if rr_type == RoundRobinType.MANHATTAN:
        return "Manhattan Distance"
    if rr_type == RoundRobinType.SORTED:
        return "Sorted"
    if rr_type == RoundRobinType.EIGHTY_FIVE_PERCENT:
        return "Yosef"
    return "Traditional Round Robin"

def plot_graph_analysis(analysis_groupings: dict[str, AnalysisGroup], graph_file_name: str, graph_name: str, graph_type: GraphType ) -> None:
    colored_line_plots : dict[str, LinePlot] = {}

    curr_x = 0
    for key, value in analysis_groupings.items():
        for analysis_data in value.get_data_group():
            data_color = get_plot_color_by_rr_type(analysis_data.rr_type)
            data_line_plot: LinePlot 
            if data_color in colored_line_plots:
                data_line_plot = colored_line_plots[data_color]
            else:
                legend = get_legend_name_by_rr_type(analysis_data.rr_type)
                data_line_plot = LinePlot(data_color, legend)
                colored_line_plots[data_color] = data_line_plot
            x = round(analysis_data.total_time_taken)
            if graph_type == GraphType.CONTEXT_SWITCH:
                data_line_plot.add_plot(x, round(analysis_data.context_switch_count))
            elif graph_type == GraphType.WAITING_TIME:
                data_line_plot.add_plot(x, round(analysis_data.average_waiting_time))
            elif graph_type == GraphType.RESPONSE_TIME:
                data_line_plot.add_plot(x, round(analysis_data.average_response_time))
            elif graph_type == GraphType.TURNAROUND_TIME:
                data_line_plot.add_plot(x, round(analysis_data.average_turnaround_time))
  
    plt.figure(figsize=(10, 6))
    legends: list[str] = []

    for key, value in colored_line_plots.items():
        plt.plot(value.get_x_plots(), value.get_y_plots(), value.color)
        legends.append(value.legend)

    legend_location = "upper right"
    plt.legend(legends, loc=legend_location)

    plt.title(graph_name)
    plt.savefig(graph_file_name)
