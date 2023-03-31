extends Node

const GodotMobileMidi = preload("res://lib/libgmm.gdns")
onready var gmm = GodotMobileMidi.new()

func _ready():
    pass

func get_platform_name():
    return gmm.get_platform_name()
