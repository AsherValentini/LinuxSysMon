from PyQt5 import QtCore, QtGui, QtWidgets
from gauge_widget import GaugeWidget, IGauge


# abstract base creator 
class GaugeCreator: 
    def createGauge(self) -> IGauge: 
        pass

# polymorphic creator for memory guage
class MemoryGaugeCreator(GaugeCreator):
    def createGauge(self) -> IGauge:
        return GaugeWidget("Memory Usage", "#0796FF", 0, 100)
    
# polymorphic creator for cpu gauge
class CpuGaugeCreator(GaugeCreator):
    def createGauge(self) -> IGauge:
        return GaugeWidget("CPU Usage", "#2AFF9F", 0, 100)
    
# polymorphic creator for disk gauge
class DiskGaugeCreator(GaugeCreator):
    def createGauge(self) -> IGauge:
        return GaugeWidget("Disk Usage", "#FF6AC1", 0, 100)

