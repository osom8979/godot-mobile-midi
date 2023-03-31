extends Node

const GodotMobileMidi = preload("res://lib/libgmm.gdns")
onready var _gmm = GodotMobileMidi.new()

var _init: bool = false

func _ready():
    _init = _gmm.init(self)

func get_platform_name():
    return _gmm.get_platform_name() + ", Init(" + str(_init) + ")"
