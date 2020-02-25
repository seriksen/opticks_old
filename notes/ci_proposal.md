# CI integration

Requirements of Opticks;
1. NVIDIA GPU (+ CUDA and OptiX)
2. Significant externals  (`opticks-externals` - not all actually required)
    - bcm
    - glm
    - glfw
    - glew
    - gleq
    - imgui
    - assimp
    - openmesh
    - plog
    - opticksaux
    - oimplicitmesher
    - odcs
    - oyoctogl
    - ocsgbsp
    - xercesc
    - g4

## CI Runner
No git service appears to offer GPU shared runners.

Solutions? 
1. Approach Oracle/IBM/Google etc... to use cloud for CI. 
Previous experience has been very positive. 
Limitation of this approach is majority use NVIDIA Tesla P100/V100 which have limited openGL support.
However, this is what a lot of clusters use so worth investigating?

2. Use an institution cluster. Often have a range of GPUs some of which will likely be RTX.

NVIDIA driver and CUDA can be solved used standard NVIDIA [docker image](https://github.com/NVIDIA/nvidia-docker).
They have no plan to support GLX in the near future though.
Would likely want to move most externals to image first + OptiX.

## Implementation
Implementation is straight forward yaml scripts controlling building of opticks, and documentaiton as well as running 
unit tests.
Would likely want to rearrange parts of opticks.

Easiest ci is for documentation.
Would have 2 files; .git-ci.yml and requirements

```yaml
#.git-ci.yml
image: docker_image

stages:
  - external_docs
  - pages

before_script:
  - pip install -r requirements.txt

sphinx:html:
  stage: external_docs
  script:
    - make -C docs html
  artifacts:
    paths:
      - docs/_build/html

sphinx:pdf:
  stage: external_docs
  image: opticks_docs:latex
  script:
    - make -C docs latexpdf
    - cp docs/_build/latex/*.pdf .
  artifacts:
    paths:
      - "./*.pdf"

pages-dry-run:
  stage: pages
  script:
  - export ENABLE_MKDOCS_PDF=1
  - mkdocs build --verbose
  except:
    - master

pages:
  stage: pages
  script:
  - export ENABLE_MKDOCS_PDF=1
  - mkdocs build --verbose
  - cp *.pdf site/pdf/.
  - mv site public
  artifacts:
    paths:
    - public
  only:
  - master
```

```text
# requirements.txt
mkdocs==1.0.4
mkdocs-material==4.2.0
mkdocs-pdf-export-plugin==0.5.5
pymdown-extensions==6.0
sphinx==2.0.1
sphinx_rtd_theme==0.4.3
dhubbard-sphinx-markdown-tables
recommonmark==0.5.0
WeasyPrint==49
```

For optickscore + , use `.ci` directory with `.git-cit.yml`.
This way, can define separate stages including deployment to cvmfs if release is tagged.
Each stage can be a bash script, so can reuse a lot of what is already written.
  


## Migration to Git
Both gitlab and github are both suitable.

Would suggest creating a group with opticks-externals, opticks (core + bits to use it), opticksdata all in separate 
projects.
Move to use issues rather than notes directory.


