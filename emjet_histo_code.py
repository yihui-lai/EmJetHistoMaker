"""Generates code for EmJetHistoMaker
Conventions:
user_XXX functions are used to define specfic instances of elements.
generate_XXX functions are called from cog to generate code fragments.
All other functions are used internally."""
standalone = False
if __name__=='__main__':
    print "Running standalone mode - Printing to screen"
    standalone = True
else:
    try: import cog
    except: ImportError

def pad_str(input_string, length=20):
    '''Pads input string with spaces (on the right hand side) to given length'''
    format_string = '{: <%d}' % length
    output_string = format_string.format(input_string)
    return output_string

def outputline(line):
    if not standalone:
        cog.outl(line)
    else:
        print(line)

############################################################
# Object definitions
############################################################
# namedtuple objects can be used as lightweight classes - similar to passive structs in C++
from collections import namedtuple
Bins    = namedtuple('Bins'    , ['nBins' , 'min'      , 'max'     , ]) # Fixed width binning
VBins   = namedtuple('VBins'   , ['nBins' , 'binedges' , 'binname' , ]) # Variable width binning - must come with name since variable width bins are specified by passing an array
Histo1F = namedtuple('Histo1F' , ['name'  , 'binsX'     , ])           # binsX or binsY contains objects of type Bins or VBins
Histo2F = namedtuple('Histo2F' , ['name'  , 'binsX'    , 'binsY'   , ])

def compute_fixed_bins(nBins, xMin, xMax):
    delta = xMax - xMin
    binWidth = delta/nBins
    binLowEdges = [(xMin + binWidth * i) for i in xrange(nBins+1)]
    return binLowEdges

def get_type_str(obj):
    if   type(obj).__name__ == 'Histo1F': return 'TH1F'
    elif type(obj).__name__ == 'Histo2F': return 'TH2F'

def histo_combine1Dto2D(histoX, histoY):
    name = '%s_VS_%s' % (histoY.name, histoX.name)
    newhisto = Histo2F(name,
            binsX = histoX.binsX,
            binsY = histoY.binsX, )
    return newhisto

def clone_object(object, prefix='', postfix=''):
    """Clone input namedtuple object that has attribute \"name\", by adding prefix and postfix to object.name"""
    if (not prefix) and (not postfix):
        print 'clone_object: Warning! Either prefix or postfix should be provided.'
        return object
    else:
        prefix_str  = ''
        postfix_str  = ''
        if prefix  : prefix_str  = '%s_' % prefix
        if postfix : postfix_str = '_%s' % postfix
    newname = '%s%s%s' % (prefix_str, object.name, postfix_str)
    output = object._replace(name=newname)
    return output

def vectorize_histo(histo, prefixlist=[''], postfixlist=['']):
    """prefix/postfix list can be a list of strings or numbers. (Numbers automatically get converted to strings.)"""
    output = []
    for prefix in prefixlist:
        if not isinstance(prefix, str): prefix = str(prefix) # Convert to string
        for postfix in postfixlist:
            if not isinstance(postfix, str): postfix = str(postfix) # Convert to string
            histo_clone = clone_object(histo, prefix=prefix, postfix=postfix)
            output.append(histo_clone)
    return output

def user_define_bins():
    import math
    """Define bins to be used for variable width histograms in this function"""
    binning_dict = {}
    binedges = map(lambda x: 10**x, compute_fixed_bins(20, -3., +2.))
    binning_dict['vertex_Lxy'] = VBins(100, binedges, 'vertex_Lxy')
    return binning_dict

def user_define_histos():
    """Define histograms in this function"""
    vbins = user_define_bins()
    histo_dict = {}
    histo_dict['jet_pt']     = Histo1F('jet_pt'     , Bins(100 , 0 , 1000.) )
    histo_dict['vertex_Lxy'] = Histo1F('vertex_Lxy' , vbins['vertex_Lxy']   )
    return histo_dict

def user_define_histo_vectors():
    """Define histogram vectors in this function"""
    histo_dict = user_define_histos()
    histo_vector_dict = {}
    histo_vector_dict['jet_pt_sorted_by_pt'] = vectorize_histo(histo_dict['jet_pt'], postfixlist=range(4))
    return histo_vector_dict

def construct_bin_str(bins):
    # Fixed width binning
    if type(bins).__name__ == 'Bins':
        return ', {nBins}, {min}, {max}'.format(nBins=bins.nBins, min=bins.min, max=bins.max)
    # Variable width binning
    if type(bins).__name__ == 'VBins':
        return ', {nBins}, nBins_{binname}, bins_{binname}'.format(nBins=bins.nBins, binname=bins.binname)

def construct_histo_init(obj):
    name    = obj.name
    typestr = get_type_str(obj)
    binstr  = construct_bin_str(obj.binsX)
    if hasattr(obj, 'binsY'): binstr += construct_bin_str(obj.binsY)
    if hasattr(obj, 'binsZ'): binstr += construct_bin_str(obj.binsZ)
    output = '{name} = new {typestr}("{name}", "{name}" {binstr});'.format(name=name, typestr=typestr, binstr=binstr)
    return output

def generate_vbin_decl():
    for name, bins in user_define_bins().iteritems():
        nBins=bins.nBins
        bin_array_str = str(bins.binedges)[1:-1] # Remove first and last character (square brackets)
        bin_array_str = '{ ' + bin_array_str + ' }'
        outputline( 'const nBins_{binname} = {nBins}; bins_{binname} = {binedges};'.format(nBins=bins.nBins, binname=bins.binname, binedges=bin_array_str) )

def generate_histo_decl():
    """Outputs lines like:
    TH1F* jet_pt;
    """
    for name, histo in user_define_histos().iteritems():
        name    = histo.name
        typestr = get_type_str(histo)
        outputline( '{typestr}* {name};'.format(name=name, typestr=typestr) )

def generate_histo_init():
    for name, histo in user_define_histos().iteritems():
        outputline( construct_histo_init(histo) )

def generate_histo_dest():
    """Outputs lines like:
    delete jet_pt;
    """
    for name, histo in user_define_histos().iteritems():
        name    = histo.name
        typestr = get_type_str(histo)
        outputline( 'delete {name};'.format(name=name, typestr=typestr) )

def generate_histo_vector_decl():
    """Outputs lines like:
    vector<TH1F*> jet_pt_sorted_by_pt;
    """
    for name, histo_vector in user_define_histo_vectors().iteritems():
        typestr = get_type_str(histo_vector[0])
        outputline( 'vector<{typestr}*> {name};'.format(name=name, typestr=typestr) )

def generate_histo_vector_init():
    for name, histo_vector in user_define_histo_vectors().iteritems():
        outputline('{')
        vector_name    = name
        for histo in histo_vector:
            histo_name = histo.name
            outputline( "  auto " + construct_histo_init(histo) )
            outputline( '  {vector_name}.push_back({histo_name});'.format(histo_name=histo_name, vector_name=vector_name) )
        outputline('}')

def generate_histo_vector_dest():
    for name, histo_vector in user_define_histo_vectors().iteritems():
        outputline( 'for (auto i: {name}) {{ delete i; }}'.format(name=name) )
        outputline( '{name}.clear();'.format(name=name) )

if __name__=='__main__':
    print 'generate_vbin_decl():'
    generate_vbin_decl()
    print''
    print 'generate_histo_decl():'
    generate_histo_decl()
    print''
    print 'generate_histo_init():'
    generate_histo_init()
    print''
    print 'generate_histo_dest():'
    generate_histo_dest()
    print''
    print 'generate_histo_vector_decl():'
    generate_histo_vector_decl()
    print''
    print 'generate_histo_vector_init():'
    generate_histo_vector_init()
    print''
    print 'generate_histo_vector_dest():'
    generate_histo_vector_dest()
    print''

