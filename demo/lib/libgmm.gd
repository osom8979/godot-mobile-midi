extends Node

const GodotMobileMidi = preload("res://lib/libgmm.gdns")
onready var _gmm = GodotMobileMidi.new()

var _init: bool = false

signal on_read_midi(v0, v1)


func _ready():
    _init = _gmm.init(self, "_on_read_midi")


func _on_read_midi(v0, v1):
    emit_signal("on_read_midi", v0, v1)


func hello_world():
    return "Hello, " + _gmm.get_platform_name() + ", Init(" + str(_init) + ")"


func devices():
    return _gmm.get_devices()
