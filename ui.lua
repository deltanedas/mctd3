if not SimpleUI then
	error("Missing SimpleUI table.", 2)
	os.exit(1)
end

for i,v in pairs(SimpleUI) do
	_ENV[i] = v
end

local label = Frame()
local size = Size()
	size.setScale(Vec2(0.4, 0.1))
	label.setSize(size)
local pos = Size()
	pos.setScale(Vec2(0.3, 0.45))
	label.setPosition(pos)
local text = Text(Fonts.minecraftia, "click block to mine it")
	text.setScaleX()
	text.setScaleY()
	label.setText(text)
local texture = Texture()
	local colour = Colour(255, 128, 255, 100)
	texture.setColour(colour)
	label.setTexture(texture)
label.setVisible()
label.setAnchored()
loggerf("done label shit", Level.DEBUG)
