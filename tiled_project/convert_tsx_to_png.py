
import xml.etree.ElementTree as ET
from PIL import Image
import os

# === CONFIG ===
# Path to your .tsx file
TSX_PATH = "./pokemon_crystal_text_tileset.tsx"
# Output file name
OUTPUT_PNG = "./pokemon_crystal_text_tileset.png"

# === LOAD TSX ===
tree = ET.parse(TSX_PATH)
root = tree.getroot()

tilewidth = int(root.attrib["tilewidth"])
tileheight = int(root.attrib["tileheight"])
margin = int(root.attrib.get("margin", 0))
spacing = int(root.attrib.get("spacing", 0))

image_elem = root.find("image")
image_path = image_elem.attrib["source"]

# Resolve relative paths
tsx_dir = os.path.dirname(TSX_PATH)
image_path = os.path.join(tsx_dir, image_path)

# === LOAD IMAGE ===
tileset_img = Image.open(image_path)
img_w, img_h = tileset_img.size

# === CALCULATE TILES ===
cols = (img_w - margin + spacing) // (tilewidth + spacing)
rows = (img_h - margin + spacing) // (tileheight + spacing)

# === CREATE NEW IMAGE ===
output_img = Image.new("RGBA", (cols * tilewidth, rows * tileheight))

# === CROP & PASTE TILES ===
for row in range(rows):
    for col in range(cols):
        x = margin + col * (tilewidth + spacing)
        y = margin + row * (tileheight + spacing)
        tile = tileset_img.crop((x, y, x + tilewidth, y + tileheight))
        output_img.paste(tile, (col * tilewidth, row * tileheight))

# === SAVE RESULT ===
output_img.save(OUTPUT_PNG)
print(f"Saved cleaned tileset to {OUTPUT_PNG}")
