from PIL import Image
import os

try:
    save_path = '/home/sacriphanius/Opt/Momentum-Firmware/applications_user/IR_Jammer/icon.png'
    img = Image.new('1', (10, 10), color=0)
    img.save(save_path)
    print(f"Icon generated successfully at {save_path}")
except ImportError:
    print("PIL not found")
except Exception as e:
    print(f"Error: {e}")
