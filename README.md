<div id="table-of-contents">
<h2>Table of Contents</h2>
<div id="text-table-of-contents">
<ul>
<li><a href="#orgb72093e">1. EmJetHistoMaker</a>
<ul>
<li><a href="#orgeb8b818">1.1. Instructions</a>
<ul>
<li><a href="#org625d3f5">1.1.1. To checkout latest code:</a></li>
<li><a href="#org426c4c9">1.1.2. To build binary</a></li>
</ul>
</li>
</ul>
</li>
</ul>
</div>
</div>

<a id="orgb72093e"></a>

# EmJetHistoMaker

Histogram making code for Emerging Jet Analysis


<a id="orgeb8b818"></a>

## Instructions


<a id="org625d3f5"></a>

### To checkout latest code:

    git clone git@github.com:yhshin11/EmJetHistoMaker.git
    cd EmJetHistoMaker
    # Checkout appropriate branch, e.g.
    git checkout dev_refactor


<a id="org426c4c9"></a>

### To build binary

EmJetHistomaker does not require CMSSW, but it does require a recent version of GCC. The simplest way to obtain this is to do \(cmsenv\) in a recent CMSSW release, e.g. \(CMSSW_7_6_3\).
For example, if you have an existing \(CMSSW_7_6_3\) release area in your home directory, you can do the following:

    cd ~/CMSSW_7_6_3/src
    cmsenv

Then return to the EmJetHistoMaker directory and run the build script:

    ./build.sh

