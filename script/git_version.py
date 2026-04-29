import subprocess
import os

ROOT_DIR = os.path.abspath(os.curdir)
GIT_PATH = os.path.join(os.getenv('USERPROFILE'), 'AppData', 'Local', 'Programs', 'Git', 'cmd', 'git.exe')
TARGET_FILE = os.path.join(ROOT_DIR, 'include/version.h')

def run_git_command(command):
    """ Run a git command and return its output. """
    try:
        result = subprocess.run(
            [GIT_PATH] + command,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            check=True,
            text=True
        )
        return result.stdout.strip()
    except subprocess.CalledProcessError as e:
        return None

def get_commit_hash():
    """ Get the current commit hash. """
    result = run_git_command(['rev-parse', 'HEAD'])

    if(result != None):
        print(f"Git hash:\t\t\033[0;32m{result[:7]}\033[0m")
        return result[:7]

    print("Git hash:\t\t\033[0;33mNone\033[0m")
    return result

def get_build_tag():
    """ Get the build tag if available (assuming it is stored as a Git tag). """
    result = run_git_command(['describe', '--tags', '--abbrev=0'])

    if(result != None):
        print(f"Git tag:\t\t\033[0;32m{result}\033[0m")
        return result

    print("Git tag:\t\t\033[0;33mNone\033[0m")
    return result

def write_version_file(version_data):
    """ Write the version data to the target file. """
    with open(TARGET_FILE, 'w') as file:
        file.write(f'#ifndef VERSION_H_\n#define VERSION_H_\n\n')
        file.write(f'#define BUILD_TAG "{version_data["build_tag"]}"\n')
        file.write(f'#define COMMIT_HASH "{version_data["commit_hash"]}"\n')
        file.write(f'\n#endif')

version_data = {
    "build_tag": get_build_tag(),
    "commit_hash": get_commit_hash(),
}

write_version_file(version_data)