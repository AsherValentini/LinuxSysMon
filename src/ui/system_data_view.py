from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QVBoxLayout, QFrame


class SystemDataView(QtWidgets.Qframe):
    def __init__(self, resource_name):
        super(SystemDataView, self).__init__()
        self._resource_name = resource_name
        self.initUI()

    def initUI(self):
        layout = QVBoxLayout(self)
        self.setLayout(layout)
        layout.setAlignment(QtCore.Qt.AlignCenter)

        # region : resource gauge 

        # endregion


