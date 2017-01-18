<div id="table-of-contents">
<h2>Table of Contents</h2>
<div id="text-table-of-contents">
<ul>
<li><a href="#orgfe6306d">1. EmJetHistoMaker</a>
<ul>
<li><a href="#orgcc8e7e8">1.1. Instructions</a>
<ul>
<li><a href="#org0f11723">1.1.1. To checkout latest code:</a></li>
<li><a href="#org0ac2084">1.1.2. To build binary</a></li>
</ul>
</li>
</ul>
</li>
</ul>
</div>
</div>

<a id="orgfe6306d"></a>

# EmJetHistoMaker

Histogram making code for Emerging Jet Analysis


<a id="orgcc8e7e8"></a>

## Instructions


<a id="org0f11723"></a>

### To checkout latest code:

    git clone git@github.com:yhshin11/EmJetHistoMaker.git
    cd EmJetHistoMaker
    # Checkout appropriate branch, e.g.
    git checkout dev_refactor


<a id="org0ac2084"></a>

### To build binary

EmJetHistomaker does not require CMSSW, but it does require a recent version of GCC. The simplest way to obtain this is to do \`cmsenv\` in a recent CMSSW release, e.g. CMSSW<sub>7</sub><sub>6</sub><sub>3</sub>.
For example, if you have an existing CMSSW<sub>7</sub><sub>6</sub><sub>3</sub> release area in your home directory, you can do the following:

    cd ~/CMSSW_7_6_3/src
    cmsenv

Then return to the EmJetHistoMaker directory and run the build script:

    ./build.sh

