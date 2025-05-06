from PyQt5 import QtWidgets, QtCore 
from PyQt5.QtWidgets import QMainWindow, QVBoxLayout

from gauge_factory import MemoryGaugeCreator, CpuGaugeCreator, DiskGaugeCreator

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("System Monitor")

        # Set the central widget 
        central_widget = QtWidgets.QWidget()
        layout = QtWidgets.QGridLayout(central_widget)

        # Create three gauges using factory
        memory_gauge_creator = MemoryGaugeCreator()
        cpu_gauge_creator = CpuGaugeCreator()
        disk_gauge_creator = DiskGaugeCreator()

        self.memory_gauge = memory_gauge_creator.createGauge()
        self.cpu_gauge = cpu_gauge_creator.createGauge()
        self.disk_gauge = disk_gauge_creator.createGauge()

        self.memory_gauge.setValue(65.0)
        self.cpu_gauge.setValue(42.5)
        self.disk_gauge.setValue(78.9)


        layout.addWidget(self.memory_gauge, 0, 0)
        layout.addWidget(self.cpu_gauge, 0, 1)
        layout.addWidget(self.disk_gauge, 0, 2)

        self.setCentralWidget(central_widget)

        self.apply_styles()
    
    def apply_styles(self):
        try: 
            with open("styles.qss", "r") as f:
                self.setStyleSheet(f.read())
        except Exception as e: 
            print("Style load error: ", e)
