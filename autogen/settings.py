from dataclasses import dataclass, field
from pathlib import Path
from typing import (
    List,
)

from .git import repo_path

@dataclass
class TemplateSpec:
    key: str
    namespace: List[str]
    install: bool = False
    src_dir: str = 'src'
    git_path: str = None

    @property
    def git_root(self):
        return git_path or '/'.join(namespace)

@dataclass
class Settings:
    root_repo: str = None
    utils_ns: str = None
    specs: List[TemplateSpec] = field(default_factory = lambda: [
        TemplateSpec(key='api', namespace = [ 'common', 'api', 'uci']),
        TemplateSpec(key='asb', namespace = [ 'common', 'iface', 'asb']),
        TemplateSpec(key='python_bindings', namespace = ['bindings'], src_dir='cpp', git_path = 'common/iface/uci_python'),
        TemplateSpec(key='protobuf', namespace = [ 'common', 'api', 'abb_protobuf']),
        TemplateSpec(key='abb', namespace = [ 'common', 'iface', 'abb_protobuf']),
    ])

    @property
    def output_root(self):
        if self.root_repo:
            return Path(self.root_repo)
        else:
            repo_match = repo_path()
            if not repo_match:
                print('Git missing, or not running in git repo to find relative repo path, try setting env var AG_GIT_ROOT.')
                return None
            candidate = Path('.').absolute()
            for p in candidate.parents:
                check = str(candidate.relative_to(p)).split('/')
                if check == repo_match:
                    print(f'Discovered a relative git root at: {p}')
                    return p
        return None