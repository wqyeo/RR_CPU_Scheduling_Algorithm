import matplotlib.pyplot as plt

from models.analysis_group import AnalysisGroup
from models.round_robin_type import RoundRobinType
from models.analysis_data import AnalysisData
from models.line_plot import LinePlot

def get_plot_color_by_rr_type(rr_type: RoundRobinType) -> str:
    if rr_type == RoundRobinType.BEST_QUANTUM_TIME:
        return "b"
    if rr_type == RoundRobinType.MANHATTAN:
        return "r"
    if rr_type == RoundRobinType.ROUND_ROBIN:
        return "g"
    return "c"

def get_legend_name_by_rr_type(rr_type: RoundRobinType) -> str:
    if rr_type == RoundRobinType.BEST_QUANTUM_TIME:
        return "Best Quantum Time"
    if rr_type == RoundRobinType.MANHATTAN:
        return "Manhattan Distance"
    return "Standard Round Robin"

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
                legend = get_legend_name_by_rr_type(analysis_data.rr_type)
                data_line_plot = LinePlot(data_color, legend)
                colored_line_plots[data_color] = data_line_plot
            data_line_plot.add_plot(curr_x, round(analysis_data.context_switch_count))
        curr_x += 1
  
    # TODO: Caller should pass in a configuration for plotting. 
    # Able to set title, size and what to plot.
    plt.figure(figsize=(10, 6))
    legends: list[str] = []
    for key, value in colored_line_plots.items():
        plt.plot(value.get_x_plots(), value.get_y_plots(), value.color)
        legends.append(value.legend)

    legend_location = "upper right"
    plt.legend(legends, loc=legend_location)
    # NOTE: Remove the X-ticks as they show nothing useful.
    # Though we can change it up such that it can show something like: "Turnaround time (y) over number of processes (x)"
    plt.xticks([])

    plt.title("Context Switch Count")
    plt.savefig(graph_file_name)
