from PIL import Image
import os
import glob
import sys


files_to_skip = ["data/blank_mod/gfx/leaders/Generic/r56_portrait_generic_admiral_south_america_1.dds", "data/blank_mod/gfx/leaders/Generic/r56_portrait_generic_middle_east_1.dds", "data/blank_mod/gfx/leaders/Generic/r56_portrait_generic_politician_asia_1_communism.dds", "data/blank_mod/gfx/leaders/Generic/r56_portrait_generic_politician_asia_1_democracy.dds", "data/blank_mod/gfx/leaders/Generic/r56_portrait_generic_politician_asia_1_democracy.dds", "data/blank_mod/gfx/leaders/Generic/r56_portrait_generic_politician_asia_1_fascism.dds", "data/blank_mod/gfx/leaders/Generic/r56_portrait_generic_politician_asia_2_communism.dds", "data/blank_mod/gfx/leaders/Generic/r56_portrait_generic_politician_asia_2_democracy.dds", "data/blank_mod/gfx/leaders/Generic/r56_portrait_generic_politician_asia_2_fascism.dds", "data/blank_mod/gfx/leaders/Generic/r56_portrait_generic_politician_south_america_2.dds"]


def DetermineSmallFilename(big_filename):
    small_filename = "data/blank_mod/gfx/interface/ideas/idea_" + os.path.basename(big_filename).replace("Portrait_", "").replace("portrait_", "")
    small_filename = small_filename.replace("portrait_", "idea_")
    return small_filename

def GetScalingFactor(width):
    return 34.0 / width;

def GetDefinition(filename):
    definition = "\tspriteType = {\n"
    definition += "\t\tname = GFX_" + os.path.basename(filename).replace("Portrait_", "").replace("portrait_", "").replace(".tga","").replace(".dds","").replace(" ", "_") + "\n"
    definition += "\t\ttexturefile = \"" + filename.replace("data/blank_mod/", "").replace("\\","/") + "\"\n"
    definition += "\t}\n"
    definition += "\n"
    definition += "\tspriteType = {\n"
    definition += "\t\tname = GFX_" + os.path.basename(filename).replace("Portrait_", "").replace("portrait_", "").replace(".tga","").replace(".dds","").replace(" ", "_") + "_small\n"
    definition += "\t\ttexturefile = \"" + DetermineSmallFilename(filename).replace("data/blank_mod/", "").replace("\\","/") + "\"\n"
    definition += "\t}\n"
    definition += "\n"
    return definition

def CreateSmallVersion(filename):
    small_filename = DetermineSmallFilename(filename)
    big_image = Image.open(filename)
    resized = big_image.resize((34, int(GetScalingFactor(big_image.size[0]) * big_image.size[1])))
    new = Image.new("RGBA", (65, 67))
    new.paste(resized, (15,int((67-resized.size[1])/2)))
    rotated = new.rotate(5)
    on_canvas = Image.open("data/resources/images/nat_ideas_template_canvas.png")
    on_canvas.paste(rotated,(-6,-4),rotated)
    frame = Image.open("data/resources/images/nat_ideas_template_frame.png")
    on_canvas.paste(frame, (0,0), frame)
    on_canvas.save(small_filename)

def UpdateMappings(filename):
    to_replace = '\"' + filename.replace('data/blank_mod/', '') + '\"'
    replacement = "GFX_" + os.path.basename(filename).replace("Portrait_", "").replace("portrait_", "").replace(".tga","").replace(".dds","")
    mappings_file = open("data/configurables/culture_graphics.txt", "r")
    mappings_lines = mappings_file.read()
    mappings_file.close()
    new_mappings_file = open("data/configurables/culture_graphics.txt", "w")
    replacement_lines = mappings_lines.replace(to_replace, replacement)
    new_mappings_file.write(replacement_lines)
    new_mappings_file.close()


gfx_file = open("data/blank_mod/interface/_leader_portraits_mod_generated.gfx", "w")
gfx_file.write("spriteTypes = {\n")

for image_file in glob.iglob('data/blank_mod/gfx/leaders/**/*', recursive=True):
    if(os.path.isdir(image_file)):
        continue
    if image_file in files_to_skip:
        continue
    CreateSmallVersion(image_file)
    gfx_file.write(GetDefinition(image_file))
    UpdateMappings(image_file.replace('data/blank_mod/', ''))

gfx_file.write("}")
gfx_file.close()
