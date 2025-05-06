import sys
from PyQt5.QtWidgets import QApplication
from main_window import MainWindow

app = QApplication(sys.argv)
window = MainWindow()
window.resize(1000, 2000)
window.show()   


sys.exit(app.exec_())

