class LinePlot:
    def __init__(self, color: str) -> None:
        self._x_axis: list[int] = []
        self._y_axis: list[int] = []
        self._colors_axis: list[str] = []
        self._color = color
        pass

    def add_plot(self, x:int, y:int) -> None:
        self._x_axis.append(x)
        self._y_axis.append(y)
        self._colors_axis.append(self._color)
        pass

    def get_x_plots(self) -> list[int]:
        return self._x_axis

    def get_y_plots(self) -> list[int]:
        return self._y_axis

    def get_color_plots(self) -> list[str]:
        return self._colors_axis
