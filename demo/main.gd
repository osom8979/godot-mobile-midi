extends MarginContainer

onready var rtl = $HBoxContainer/RichTextLabel
onready var gmm = $libgmm

# Called when the node enters the scene tree for the first time.
func _ready():
    pass

func _on_Button_pressed():
    rtl.append_bbcode(gmm.hello_world() + "\n")
