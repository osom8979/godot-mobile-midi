extends MarginContainer

onready var rtl = $HBoxContainer/RichTextLabel
onready var gmm = $libgmm


func _ready():
    pass


func _on_HelloButton_pressed():
    rtl.append_bbcode(gmm.hello_world() + "\n")


func _on_DeviceCountButton_pressed():
    rtl.append_bbcode(str(gmm.devices()) + "\n")
