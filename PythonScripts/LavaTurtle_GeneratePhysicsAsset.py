import unreal

SKELETAL_MESH_PATH = "/Game/Blueprints/Character/TestAI_Characters/LavaTurtle5/SKM_LavaTurtle"

mesh = unreal.load_asset(SKELETAL_MESH_PATH)

if not mesh:
    raise Exception("Could not load skeletal mesh")

physics_asset = unreal.EditorPhysicsAssetLibrary.create_physics_asset(
    mesh
)

print("Physics asset created:", physics_asset)

# =====================================================
# ASSIGN TO MESH
# =====================================================

mesh.set_editor_property("physics_asset", physics_asset)

# =====================================================
# BODY SETUP HELPERS
# =====================================================

def add_capsule(body_setup, radius, length):
    sphyl = unreal.KSphylElem()
    sphyl.radius = radius
    sphyl.length = length
    body_setup.agg_geom.sphyl_elems.append(sphyl)


def add_box(body_setup, x, y, z):
    box = unreal.KBoxElem()
    box.x = x
    box.y = y
    box.z = z
    body_setup.agg_geom.box_elems.append(box)
    
# =====================================================
# PERFORMANCE-FRIENDLY BODY MAP
# =====================================================

BODY_CONFIG = {
    "pelvis": ("capsule", 18, 24),
    "spine_01": ("capsule", 14, 18),
    "spine_02": ("capsule", 14, 18),
    "spine_03": ("capsule", 16, 20),
    "neck_01": ("capsule", 8, 10),
    "head": ("capsule", 12, 16),

    "upperarm_l": ("capsule", 7, 24),
    "lowerarm_l": ("capsule", 6, 22),
    "hand_l": ("box", 10, 5, 4),

    "upperarm_r": ("capsule", 7, 24),
    "lowerarm_r": ("capsule", 6, 22),
    "hand_r": ("box", 10, 5, 4),

    "thigh_l": ("capsule", 10, 32),
    "calf_l": ("capsule", 8, 28),
    "foot_l": ("box", 14, 8, 5),

    "thigh_r": ("capsule", 10, 32),
    "calf_r": ("capsule", 8, 28),
    "foot_r": ("box", 14, 8, 5),
}

# =====================================================
# CREATE BODY SETUPS
# =====================================================

for bone_name, config in BODY_CONFIG.items():

    body_setup = unreal.PhysicsAssetUtils.create_new_body(
        physics_asset,
        bone_name
    )

    if not body_setup:
        print(f"Failed to create body for {bone_name}")
        continue

    shape_type = config[0]

    if shape_type == "capsule":
        add_capsule(body_setup, config[1], config[2])

    elif shape_type == "box":
        add_box(body_setup, config[1], config[2], config[3])

    body_setup.default_instance.set_editor_property(
        "linear_damping",
        0.2
    )

    body_setup.default_instance.set_editor_property(
        "angular_damping",
        2.0
    )

# =====================================================
# SHELL CONFIGURATION
# =====================================================

SHELL_BONES = [
    "shell",
    "carapace",
    "back_shell"
]

for shell_bone in SHELL_BONES:

    shell_body = physics_asset.find_body_setup(shell_bone)

    if shell_body:

        shell_body.default_instance.set_editor_property(
            "physics_type",
            unreal.PhysicsType.KINEMATIC
        )

        shell_body.default_instance.set_editor_property(
            "mass_scale",
            4.0
        )

# =====================================================
# SAVE ASSET
# =====================================================

unreal.EditorAssetLibrary.save_loaded_asset(physics_asset)
unreal.EditorAssetLibrary.save_loaded_asset(mesh)

print("Physics Asset generation complete")
