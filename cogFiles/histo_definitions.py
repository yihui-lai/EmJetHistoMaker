"""Generates code for EmJetHistoMaker
Conventions:

user_XXX functions are used to define specfic instances of elements.
generate_XXX functions are called from cog to generate code fragments.
All other functions are used internally."""

from histo_functions import *

standalone = False
if __name__=='__main__':
    print "Running standalone mode - Printing to screen"
    standalone = True
else:
    try: import cog
    except: ImportError

def outputline(line):
    if not standalone:
        cog.outl(line)
    else:
        print(line)

############################################################
# Object definitions
############################################################
# namedtuple objects can be used as lightweight classes - similar to passive structs in C++

def user_define_bins():
    import math
    """Define bins to be used for variable width histograms in this function"""
    binning_dict = OrderedDict()
    binedges = map(lambda x: 10**x, compute_fixed_bins(100, -8., +2.))
    name = 'track_ipXY'          ; binning_dict[name] = VBins(len(binedges)-1, binedges, name)
    binedges = map(lambda x: 10**x, compute_fixed_bins(100, -2., +2.))
    name = 'track_ipXYSig'       ; binning_dict[name] = VBins(len(binedges)-1, binedges, name)
    binedges = map(lambda x: 10**x, compute_fixed_bins(100, -3., +2.))
    name = 'track_distanceToJet' ; binning_dict[name] = VBins(len(binedges)-1, binedges, name)
    binedges = map(lambda x: 10**x, compute_fixed_bins(20, -3., +2.))
    name = 'vertex_Lxy'          ; binning_dict[name] = VBins(len(binedges)-1, binedges, name)
    name = 'vertex_mass'         ; binning_dict[name] = VBins(len(binedges)-1, binedges, name)
    return binning_dict

def user_define_histos():
    """Define histograms in this function"""
    vbins = user_define_bins()
    histo_dict = OrderedDict()
    name = 'nVtx'                      ; histo_dict[name] = Histo1F(name , Bins(100 , 0   , 100  ) );
    name = 'pv_indexInColl'            ; histo_dict[name] = Histo1F(name , Bins(  5 , 0   ,   5  ) )
    name = 'pv_z'                      ; histo_dict[name] = Histo1F(name , Bins( 60 ,-30  ,  30  ) )
    name = 'ht'                        ; histo_dict[name] = Histo1F(name , Bins(100 , 0   , 2500 ) )
    name = 'ht4'                       ; histo_dict[name] = Histo1F(name , Bins(100 , 0   , 2500 ) )
    name = 'jet_pt'                    ; histo_dict[name] = Histo1F(name , Bins(102 , -20 , 1000 ) )
    name = 'jet_eta'                   ; histo_dict[name] = Histo1F(name , Bins(100 , -5  , 5    ) )
    name = 'jet_phi'                   ; histo_dict[name] = Histo1F(name , Bins(100 , -5  , 5    ) )
    name = 'jet_nTrack'                ; histo_dict[name] = Histo1F(name , Bins(100 , 0.  , 100  ) )
    name = 'jet_nTrack_pt1000'         ; histo_dict[name] = Histo1F(name , Bins(100 , 0.  , 100  ) )
    #name = 'jet_medianIP'              ; histo_dict[name] = Histo1F(name , vbins['track_ipXY']     )
    #name = 'jet_maxIP'                 ; histo_dict[name] = Histo1F(name , vbins['track_ipXY']     )
    name = 'jet_medianIP'              ; histo_dict[name] = Histo1F(name , Bins(200 , -0.2 , 0.2  ) )
    name = 'jet_maxIP'                 ; histo_dict[name] = Histo1F(name , Bins(200 , -0.2 , 0.2  ) )
    name = 'jet_medianIPSig'           ; histo_dict[name] = Histo1F(name , Bins(200 , -8   , 8    ) )
    name = 'jet_maxIPSig'              ; histo_dict[name] = Histo1F(name , Bins(200 , -8   , 8    ) )
    name = 'jet_alphaMax'              ; histo_dict[name] = Histo1F(name , Bins(200 , -1.  , 1.   ) )
    name = 'jet_Alpha'                 ; histo_dict[name] = Histo1F(name , Bins(100 ,  0.  , 1.   ) )
    name = 'jet_Alpha2D'               ; histo_dict[name] = Histo1F(name , Bins(100 ,  0.  , 1.   ) )
    name = 'jet_Alpha2DSig'            ; histo_dict[name] = Histo1F(name , Bins(100 ,  0.  , 1.   ) )
    name = 'jet_ltkfrac'               ; histo_dict[name] = Histo1F(name , Bins(100 ,  0.  , 2.   ) )
    name = 'jet_frac2DSig'             ; histo_dict[name] = Histo1F(name , Bins(100 ,  0.  , 1.   ) )
    name = 'jet_nonpufrac'             ; histo_dict[name] = Histo1F(name , Bins(100 ,  0.  , 1.   ) )
    name = 'jet_prompt_frac'           ; histo_dict[name] = Histo1F(name , Bins(100 , 0.  , 2.   ) )
    name = 'jet_disp_frac'             ; histo_dict[name] = Histo1F(name , Bins(100 , 0.  , 2.   ) )
    name = 'jet_cef'                   ; histo_dict[name] = Histo1F(name , Bins(100 , 0.  , 1.   ) )
    name = 'jet_nef'                   ; histo_dict[name] = Histo1F(name , Bins(100 , 0.  , 1.   ) )
    name = 'track_pt'                  ; histo_dict[name] = Histo1F(name , Bins(100 , 0.  , 10.  ) )
    name = 'track_eta'                 ; histo_dict[name] = Histo1F(name , Bins(100 , -5  , 5    ) )
    name = 'track_phi'                 ; histo_dict[name] = Histo1F(name , Bins(100 , -5  , 5    ) )
    #name = 'track_ipXY'                ; histo_dict[name] = Histo1F(name , vbins['track_ipXY']     )
    #name = 'track_ipXYSig'             ; histo_dict[name] = Histo1F(name , vbins['track_ipXYSig']  )
    name = 'track_ipXY'                ; histo_dict[name] = Histo1F(name , Bins(200 , -0.2, 0.2   ) )
    name = 'track_ipXYSig'             ; histo_dict[name] = Histo1F(name , Bins(100 , -8 , 8   ) )
    name = 'track_quality'             ; histo_dict[name] = Histo1F(name , Bins(200 , 0.  , 200  ) )

    # Define 2D histograms from ordered pairs of 1D histograms
    # combine1Dto2D( histoX, histoY)
    '''
    histo_2d_dict = OrderedDict()
    h = histo_combine1Dto2D( histo_dict['jet_alphaMax'], histo_dict['jet_medLogIpSig'], ); histo_2d_dict[h.name] = h
    h = histo_combine1Dto2D( histo_dict['jet_alphaMax'], histo_dict['jet_prompt_frac'], ); histo_2d_dict[h.name] = h
    h = histo_combine1Dto2D( histo_dict['jet_alphaMax'], histo_dict['jet_disp_frac'], ); histo_2d_dict[h.name] = h
    h = histo_combine1Dto2D( histo_dict['jet_alphaMax'], histo_dict['jet_maxIP'], ); histo_2d_dict[h.name] = h
    h = histo_combine1Dto2D( histo_dict['jet_pt'], histo_dict['jet_alphaMax'], ); histo_2d_dict[h.name] = h
    h = histo_combine1Dto2D( histo_dict['jet_pt'], histo_dict['jet_alphaMax2'], ); histo_2d_dict[h.name] = h
    h = histo_combine1Dto2D( histo_dict['jet_pt'], histo_dict['jet_Alpha'], ); histo_2d_dict[h.name] = h
    h = histo_combine1Dto2D( histo_dict['jet_pt'], histo_dict['jet_disp_frac'], ); histo_2d_dict[h.name] = h
    h = histo_combine1Dto2D( histo_dict['nVtx'], histo_dict['jet_alphaMax'], ); histo_2d_dict[h.name] = h
    h = histo_combine1Dto2D( histo_dict['ht'], histo_dict['jet_alphaMax'], ); histo_2d_dict[h.name] = h
    h = histo_combine1Dto2D( histo_dict['jet_alphaMax2'], histo_dict['jet_alphaMax'], ); histo_2d_dict[h.name] = h
    h = histo_combine1Dto2D( histo_dict['jet_alphaMax'], histo_dict['pv_indexInColl'], ); histo_2d_dict[h.name] = h
    h = histo_combine1Dto2D( histo_dict['jet_alphaMax2'], histo_dict['pv_indexInColl'], ); histo_2d_dict[h.name] = h
    h = histo_combine1Dto2D( histo_dict['jet_pt'], histo_dict['jet_N'], ); histo_2d_dict[h.name] = h
    h = histo_combine1Dto2D( histo_dict['jet_eta'], histo_dict['jet_N'], ); histo_2d_dict[h.name] = h
    h = histo_combine1Dto2D( histo_dict['jet_phi'], histo_dict['jet_N'], ); histo_2d_dict[h.name] = h
    h = histo_combine1Dto2D( histo_dict['jet_pt'], histo_dict['jet_nTrack'], ); histo_2d_dict[h.name] = h
    h = histo_combine1Dto2D( histo_dict['jet_pt'], histo_dict['jet_nTrackPostCut'], ); histo_2d_dict[h.name] = h
    histo_dict.update(histo_2d_dict)
    '''
    # Jet plot variations

    histo_clone_dict = OrderedDict()
    for name, histo in histo_dict.iteritems():
        if name[:4]=='jet_' or name[:6]=='track_':
            histo_clone = clone_object(histo, postfix='pt0')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='pt1')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='pt2')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='pt3')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='pt4')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='pt5')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='pt6')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='ptX')
            histo_clone_dict[histo_clone.name] = histo_clone
    histo_dict.update(histo_clone_dict)

    histo_clone_dict = OrderedDict()
    for name, histo in histo_dict.iteritems():
        if name[:4]=='jet_' or name[:6]=='track_':
            histo_clone = clone_object(histo, postfix='1')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='2')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='3')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='4')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='5')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='10')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='19')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='21')
            histo_clone_dict[histo_clone.name] = histo_clone
    histo_dict.update(histo_clone_dict)

    histo_clone_dict = OrderedDict()
    for name, histo in histo_dict.iteritems():
        if name[:4]=='jet_' or name[:6]=='track_':
            histo_clone = clone_object(histo, postfix='JTbasic')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='JTAlpha')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='JTAlpha2DSig')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='JTAF2DSig')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='JTbasic3')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='JTAlpha2DSig3')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='JTAF2DSig3')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='JTbasic__TypeIII')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='JTAlpha2DSig__TypeIII')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='JTAlpha__TypeIII')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='JTbasic__TypeIV')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='JTAlpha2DSig__TypeIV')
            histo_clone_dict[histo_clone.name] = histo_clone
            histo_clone = clone_object(histo, postfix='JTAlpha__TypeIV')
            histo_clone_dict[histo_clone.name] = histo_clone
    histo_dict.update(histo_clone_dict)

    # event-level plot variations
    #histo_clone_dict = OrderedDict()
    #for name, histo in histo_dict.iteritems():
    #     histo_clone = clone_object(histo, postfix='HLTfiltered')
    #     histo_clone_dict[histo_clone.name] = histo_clone
         #histo_clone = clone_object(histo, postfix='Jetfiltered')
         #histo_clone_dict[histo_clone.name] = histo_clone
    #histo_dict.update(histo_clone_dict)    

    return histo_dict

def user_define_histo_vectors():
    """Define histogram vectors in this function"""
    histo_dict = user_define_histos()
    histo_vector_dict = OrderedDict()
    return histo_vector_dict

def construct_bin_str(bins):
    # Fixed width binning
    if type(bins).__name__ == 'Bins':
        return ', {nBins}, {min}, {max}'.format(nBins=bins.nBins, min=bins.min, max=bins.max)
    # Variable width binning
    if type(bins).__name__ == 'VBins':
        return ', nBins_{binname}, bins_{binname}'.format(nBins=bins.nBins, binname=bins.binname)

def construct_histo_init(obj):
    name    = obj.name
    typestr = get_type_str(obj)
    binstr  = construct_bin_str(obj.binsX)
    if hasattr(obj, 'binsY'): binstr += construct_bin_str(obj.binsY)
    if hasattr(obj, 'binsZ'): binstr += construct_bin_str(obj.binsZ)
    output = '{name} = new {typestr}("{name}", "{name}" {binstr});'.format(name=name, typestr=typestr, binstr=binstr)
    return output

def construct_histo_map_init(obj):
    name    = obj.name
    typestr = get_type_str(obj)
    mapstr  = get_map_str(obj)
    binstr  = construct_bin_str(obj.binsX)
    if hasattr(obj, 'binsY'): binstr += construct_bin_str(obj.binsY)
    if hasattr(obj, 'binsZ'): binstr += construct_bin_str(obj.binsZ)
    output = '{mapstr}["{name}"] = new {typestr}("{name}", "{name}" {binstr});'.format(name=name, typestr=typestr, mapstr=mapstr, binstr=binstr)
    return output

def generate_vbin_decl():
    for name, bins in user_define_bins().iteritems():
        nBins=bins.nBins
        bin_array_str = str(bins.binedges)[1:-1] # Remove first and last character (square brackets)
        bin_array_str = '{ ' + bin_array_str + ' }'
        outputline( 'const int nBins_{binname} = {nBins}; const double bins_{binname}[{nBins}+1] = {binedges};'.format(nBins=bins.nBins, binname=bins.binname, binedges=bin_array_str) )

def generate_histo_map_init():
    for name, histo in user_define_histos().iteritems():
        outputline( construct_histo_map_init(histo) )

def calculate_index(histo_dict):
    """Takes histo_dict and calculates index in vectors based on their order in dictionary and their type (1D vs 2D)"""
    index_1d = 0
    index_2d = 0
    histo_id_dict = OrderedDict()
    for name, histo in histo_dict.iteritems():
        if type(histo).__name__ == 'Histo1F':
            histo_id_dict[name] = index_1d
            index_1d += 1
        elif type(histo).__name__ == 'Histo2F':
            histo_id_dict[name] = index_2d
            index_2d += 1
    return histo_id_dict

def generate_histo_index():
    histo_id_dict = calculate_index( user_define_histos() )
    for name, histo in user_define_histos().iteritems():
        template_str = 'if (name=="{name}") return {index};'
        index = histo_id_dict[name]
        output_str = template_str.format(name=name, index=index)
        outputline(output_str)

if __name__=='__main__':
    print 'generate_histo_index():'
    generate_histo_index()
    print''
    print 'generate_vbin_decl():'
    generate_vbin_decl()
    print''
    print 'generate_histo_map_init():'
    generate_histo_map_init()
    print''

