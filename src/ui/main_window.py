from PyQt5.QtWidgets import QMainWindow, QTabWidget

from cpu_view import CpuView

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("System Monitor")
        self.setGeometry(100, 100, 800, 600)

        # Create a tab widget
        self.tab_widget = QTabWidget()
        
        # Add tabs
        self.cpu_view = CpuView()
        self.memory_view = CpuView()
        
        self.tab_widget.addTab(self.cpu_view, "CPU")
        self.tab_widget.addTab(self.memory_view, "Memory")

        # Set the central widget
        self.setCentralWidget(self.tab_widget)

        self.apply_styles()
    
    def apply_styles(self):
        with open("styles.qss", "r") as f:
            self.setStyleSheet(f.read())
