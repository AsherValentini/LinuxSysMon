from PyQt5.QtWidgets import QWidget, QLabel, QVBoxLayout

class CpuView(QWidget):
    def __init__(self):
        super().__init__()
        layout  = QVBoxLayout()
        layout.addWidget(QLabel("CPU View"))
        self.setLayout(layout)


