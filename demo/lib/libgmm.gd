extends Node

const Gmm = preload("res://lib/libgmm.gdns")
onready var gmm = Gmm.new()

func _ready():
    pass

func get_data():
    return gmm.get_data()
