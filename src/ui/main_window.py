from PyQt5.QtWidgets import QMainWindow, QTabWidget

from system_data_view import CpuView

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("System Monitor")
        self.setGeometry(100, 100, 800, 600)

        
        # Set the central widget
        self.setCentralWidget(self.tab_widget)

        self.apply_styles()
    
    def apply_styles(self):
        with open("styles.qss", "r") as f:
            self.setStyleSheet(f.read())
