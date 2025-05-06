from PyQt5 import QtWidgets, QtGui, QtCore
from PyQt5.QtCore import Qt
import math

#abstract product interface 
class IGauge: 
    def setValue(self, val):
        pass


#concrete product
class GaugeWidget(QtWidgets.QWidget, IGauge): 
    def __init__(self, label, color, range_min, range_max, parent=None):
        super().__init__(parent)
        self._value = 0
        self._min = 0
        self._max = 100.0
        self._label = label
        self._range_min = range_min
        self._range_max = range_max
        self._color = QtGui.QColor(color) # bar color "#0796FF"
        self._backgroundColor = QtGui.QColor("#1e1e1e")
        self.setAttribute(Qt.WA_StyledBackground, True)
        self.setLabel(self._label)
        self.setRange(range_min, range_max)

    def setValue(self, val):
        self._value = max(self._min, min(val, self._max))
        self.update()

    def setRange(self, minimum, maximum):
        self._min = minimum
        self._max = maximum
        self.update()

    def setLabel(self, text):
        self._label = text
        self.update()

    def setColor(self, color):
        self._color = QtGui.QColor(color)
        self.update()

    def paintEvent(self, event):
        painter = QtGui.QPainter(self)
        painter.setRenderHint(QtGui.QPainter.Antialiasing)

        # Use a tighter rect to prevent clipping
        margin = 16
        rect = self.rect().adjusted(margin, margin, -margin, -margin)
        size = min(rect.width(), rect.height())
        radius = size / 2 - margin

        # Raise the center slightly to avoid bottom clipping
        center = QtCore.QPointF(rect.center().x(), rect.center().y() + radius * 0.05)

        # Arc bounds
        arc_rect = QtCore.QRectF(
            center.x() - radius,
            center.y() - radius,
            2 * radius,
            2 * radius
        )

        # Arc angles
        start_angle = 300  # in degrees
        span_angle = 200   # a less full arc (Q3 to Q1, stops short of top)

        percent = (self._value - self._min) / (self._max - self._min)
        end_angle = start_angle + span_angle * percent

        # Draw arcs
        self._drawArc(painter, center, radius, start_angle, start_angle + span_angle, QtGui.QColor("#333333"))
        self._drawArc(painter, center, radius, start_angle, end_angle, self._color)

        # Draw title (left-centered above arc)
        painter.setPen(Qt.white)
        title_font = QtGui.QFont("JetBrains Mono", 10)
        painter.setFont(title_font)
        title_rect = QtCore.QRectF(rect.left(), rect.top() + 4, rect.width() / 2, 20)
        painter.drawText(title_rect, Qt.AlignCenter, self._label)

        # Draw value (centered under arc)
        value_font = QtGui.QFont("JetBrains Mono", 13, QtGui.QFont.Bold)
        painter.setFont(value_font)
        value_text = self._formatValue(self._value, self._max)
        value_rect = QtCore.QRectF(rect.left(), center.y() - 20, rect.width(), 40)
        painter.drawText(value_rect, Qt.AlignCenter, value_text)

    def _drawArc(self, painter, center, radius, start_deg, end_deg, color):
        pen = QtGui.QPen(color, 12, Qt.SolidLine, Qt.RoundCap)
        painter.setPen(pen)

        arc_rect = QtCore.QRectF(center.x() - radius, center.y() - radius, 2 * radius, 2 * radius)
        span = end_deg - start_deg
        painter.drawArc(arc_rect, int((90 - end_deg) * 16), int(span * 16))

    def _formatValue(self, val, max_val):
        return f"{val:.1f} / {max_val:.1f} GB"


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)

    gauge = GaugeWidget("Memory Usage", "#0796FF", 0, 100)
    gauge.setValue(90)
    gauge.resize(800, 700)
    gauge.show()

    sys.exit(app.exec_())

