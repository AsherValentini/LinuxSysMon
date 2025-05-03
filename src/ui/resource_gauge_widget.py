from PyQt5 import QtWidgets, QtGui, QtCore
from PyQt5.QtCore import Qt
import math

class GaugeWidget(QtWidgets.QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self._value = 0
        self._min = 0
        self._max = 100
        self._label = "CPU"
        self._color = QtGui.QColor("#0796FF")  # JetBrains-style blue
        self._backgroundColor = QtGui.QColor("#1e1e1e")  # JetBrains dark background
        self.setAttribute(Qt.WA_StyledBackground, True)
        self.setMinimumSize(200, 150)

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
        painter.fillRect(self.rect(), self._backgroundColor)

        # Geometry
        margin = 20
        radius = min(self.width(), self.height() * 2) / 2 - margin
        center = QtCore.QPointF(self.width() / 2, self.height() - margin)

        # Gauge angles
        start_angle = 150  # degrees (left side)
        span_angle = 240  # total arc
        end_angle = start_angle + span_angle
        value_percent = (self._value - self._min) / (self._max - self._min)
        value_angle = start_angle + value_percent * span_angle

        # Draw background arc
        self._drawArc(painter, center, radius, start_angle, end_angle, QtGui.QColor("#333333"))

        # Draw value arc
        self._drawArc(painter, center, radius, start_angle, value_angle, self._color)

        # Draw label
        painter.setPen(QtGui.QColor("white"))
        painter.setFont(QtGui.QFont("JetBrains Mono", 10))
        painter.drawText(self.rect().adjusted(0, 10, 0, -self.height()//3), Qt.AlignCenter, self._label)

        # Draw value text
        painter.setFont(QtGui.QFont("JetBrains Mono", 16, QtGui.QFont.Bold))
        painter.drawText(self.rect().adjusted(0, 30, 0, -self.height()//5), Qt.AlignCenter, f"{int(self._value)}%")

    def _drawArc(self, painter, center, radius, start_deg, end_deg, color):
        pen = QtGui.QPen(color, 12, Qt.SolidLine, Qt.RoundCap)
        painter.setPen(pen)

        arc_rect = QtCore.QRectF(
            center.x() - radius,
            center.y() - radius,
            2 * radius,
            2 * radius
        )

        span = end_deg - start_deg
        painter.drawArc(arc_rect, int((90 - end_deg) * 16), int(span * 16))  # Qt starts at 3 o'clock and moves counterclockwise


from PyQt5 import QtWidgets, QtGui, QtCore
from PyQt5.QtCore import Qt


class GaugeWidget2(QtWidgets.QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self._value = 0
        self._min = 0
        self._max = 100.0
        self._label = "Memory"
        self._color = QtGui.QColor("#0796FF")
        self._backgroundColor = QtGui.QColor("#1e1e1e")
        self.setAttribute(Qt.WA_StyledBackground, True)
        self.setMinimumSize(200, 150)

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
        painter.fillRect(self.rect(), self._backgroundColor)

        # Geometry
        margin = 20
        radius = min(self.width(), self.height() * 2) / 2 - margin
        center = QtCore.QPointF(self.width() / 2, self.height() - radius * 0.3)

        # Arc angles
        start_angle = 150
        span_angle = 240
        end_angle = start_angle + span_angle
        value_percent = (self._value - self._min) / (self._max - self._min)
        value_angle = start_angle + value_percent * span_angle

        # Draw background arc
        self._drawArc(painter, center, radius, start_angle, end_angle, QtGui.QColor("#333333"))

        # Draw value arc
        self._drawArc(painter, center, radius, start_angle, value_angle, self._color)

        # Fonts
        title_font = QtGui.QFont("JetBrains Mono", 10)
        value_font = QtGui.QFont("JetBrains Mono", 13, QtGui.QFont.Bold)

        # Draw title above arc
        painter.setFont(title_font)
        painter.setPen(QtGui.QColor("white"))
        title_rect = QtCore.QRectF(center.x() - radius, center.y() - radius - 30, radius * 2, 20)
        painter.drawText(title_rect, Qt.AlignLeft | Qt.AlignVCenter, self._label)

        # Draw value below arc
        painter.setFont(value_font)
        value_text = self._formatValue(self._value, self._max)
        value_rect = QtCore.QRectF(center.x() - radius, center.y() - radius * 0.2, radius * 2, 30)
        painter.drawText(value_rect, Qt.AlignHCenter | Qt.AlignTop, value_text)

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

    gauge = GaugeWidget2()
    gauge.setLabel("Memory Usage")
    gauge.setRange(0, 100)
    gauge.setValue(90)
    gauge.resize(400, 400)
    gauge.show()

    sys.exit(app.exec_())

