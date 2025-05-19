import my_module

def main():
  print("Starting another_module")

# Runs the code in main only if we run the file directly.
# Useful for situations where you're meant to run the program from 
# some entry point.
if __name__ == "__main__":
  main()