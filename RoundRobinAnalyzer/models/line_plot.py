class LinePlot:
    def __init__(self, color: str, legend: str) -> None:
        self._x_axis: list[int] = []
        self._y_axis: list[int] = []
        self.color = color
        self.legend = legend
        pass

    def add_plot(self, x:int, y:int) -> None:
        self._x_axis.append(x)
        self._y_axis.append(y)
        pass

    def get_x_plots(self) -> list[int]:
      return self._x_axis

    def get_y_plots(self) -> list[int]:
      return self._y_axis
