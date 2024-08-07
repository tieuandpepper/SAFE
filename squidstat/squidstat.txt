import sys
import struct
from PySide2.QtWidgets import QApplication
from SquidstatPyLibrary import AisDeviceTracker
from SquidstatPyLibrary import AisCompRange
from SquidstatPyLibrary import AisDCData
from SquidstatPyLibrary import AisACData
from SquidstatPyLibrary import AisExperimentNode
from SquidstatPyLibrary import AisErrorCode
from SquidstatPyLibrary import AisExperiment
from SquidstatPyLibrary import AisInstrumentHandler
from SquidstatPyLibrary import AisConstantPotElement
from SquidstatPyLibrary import AisEISPotentiostaticElement
from SquidstatPyLibrary import AisConstantCurrentElement
 
app = QApplication()

tracker = AisDeviceTracker.Instance()
 
tracker.newDeviceConnected.connect(lambda deviceName: print("Device is Connected: %s" % deviceName))
tracker.connectToDeviceOnComPort("COM7")
 
handler = tracker.getInstrumentHandler("Plus1215")
 
handler.activeDCDataReady.connect(lambda channel, data: print("timestamp:", "{:.9f}".format(data.timestamp), "workingElectrodeVoltage: ", "{:.9f}".format(data.workingElectrodeVoltage)))
handler.experimentStopped.connect(lambda channel : print("Experiment Completed: %d" % channel))
 
# create a custom experiment
experiment = AisExperiment()
# constructing a constant potential element with required arguments
# voltage: 5V, interval: 1s, duration: 10s 
cvElement = AisConstantPotElement(5, 1, 10)
# append to experiment, the created CV element and set it to run 1 time
experiment.appendElement(cvElement,1)
 
handler.uploadExperimentToChannel(0,experiment)
handler.startUploadedExperiment(0)

sys.exit(app.exec_())
print("Done")