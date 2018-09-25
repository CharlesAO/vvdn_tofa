use strict;
use POSIX;

sub ExtractNetworkData;
sub GetLayersOutputOffset;
sub EliminateSoftMax;
sub WriteCNNStruct2Blob;
sub OptimizeTempBuffers;
sub ExtractCoefficients;
sub push_bits;
sub get_struct;

my @layers;
my %Index2OptimizedIndex;

my $network_name;
my $weight_location = 0;

open my $inBlob_hnd, $ARGV[0] or die "Can't open $ARGV[0]\n";
binmode $inBlob_hnd;

ExtractNetworkData();

print "\nConverting $network_name... patience please\n";

GetLayersOutputOffset();

OptimizeTempBuffers();
EliminateSoftMax();

open my $outBlob_hnd, ">$network_name.cnnhwblob" or die "can't open output blob file for write\n";
binmode $outBlob_hnd;

WriteCNNStruct2Blob();
ExtractCoefficients();

close $inBlob_hnd;
close $outBlob_hnd;

sub ExtractNetworkData
{
    my $buffer;
    read $inBlob_hnd, $buffer, 4; #Dummy
    read $inBlob_hnd, $buffer, 4; #Dummy
    read $inBlob_hnd, $buffer, 4; #Dummy
    read $inBlob_hnd, $buffer, 4; #Dummy
    read $inBlob_hnd, $buffer, 4; #Dummy
    read $inBlob_hnd, $buffer, 4; #Dummy
    read $inBlob_hnd, $buffer, 4; #Dummy
    read $inBlob_hnd, $buffer, 4; #Dummy

    read $inBlob_hnd, $buffer, 4; #File size (not interesting)
    read $inBlob_hnd, $buffer, 4; #File version (not interesting)
    read $inBlob_hnd, $buffer, 100; #Network name
    $buffer =~ /(\w*)/;
    $network_name = $1;

    read $inBlob_hnd, $buffer, 100; #report directory (not interesting)
    read $inBlob_hnd, $buffer, 4; #Number of layers
    my $network_stage_total = unpack("V", $buffer);

    read $inBlob_hnd, $buffer, 4; #Weights offset
    $weight_location = unpack("V", $buffer); 

    #Skip over the Myriad parameters (they are not interesting)
    read $inBlob_hnd, $buffer, 2; #First shave
    read $inBlob_hnd, $buffer, 2; #Last shave
    read $inBlob_hnd, $buffer, 4; #leon Mem Location
    read $inBlob_hnd, $buffer, 4; #leon Mem Size
    read $inBlob_hnd, $buffer, 4; #DMA Link Agent   

    
    for(my $layer_index = 0; $layer_index < $network_stage_total; $layer_index++)
    {
        my $lref = {};
        read $inBlob_hnd, $buffer, 100; #Stage name
        $buffer =~ /(\w+)\/(\w+)/;
        ${$lref}{"stageName"} = $1."/".$2;
        read $inBlob_hnd, $buffer, 1;
        ${$lref}{"stageType"} = unpack("C", $buffer);
        read $inBlob_hnd, $buffer, 1;
        ${$lref}{"radixX"} = unpack("C", $buffer);
        read $inBlob_hnd, $buffer, 1;
        ${$lref}{"radixY"} = unpack("C", $buffer);
        read $inBlob_hnd, $buffer, 1;
        ${$lref}{"strideX"} = unpack("C", $buffer);
        read $inBlob_hnd, $buffer, 1;
        ${$lref}{"strideY"} = unpack("C", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"inputDimX"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"inputDimY"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"inputDimZ"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"tapDimX"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"tapDimY"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"tapDimZ"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"outputDimX"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"outputDimY"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"outputDimZ"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"inputStrideX"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"inputStrideY"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"inputStrideZ"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"tapStrideX"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"tapStrideY"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"tapStrideZ"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"outputStrideX"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"outputStrideY"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"outputStrideZ"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 1;
        ${$lref}{"padStyle"} = unpack("C", $buffer);
        read $inBlob_hnd, $buffer, 1;
        ${$lref}{"dataType"} = unpack("C", $buffer);
        read $inBlob_hnd, $buffer, 1;
        ${$lref}{"internalPrecision"} = unpack("C", $buffer);
        read $inBlob_hnd, $buffer, 1;
        ${$lref}{"storageOrder"} = unpack("C", $buffer);
        read $inBlob_hnd, $buffer, 4;
        ${$lref}{"inputPointer"} = unpack("V", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"inputIndex"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 4;
        ${$lref}{"weightsPointer"} = unpack("V", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"weightsIndex"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 4;
        ${$lref}{"biasPointer"} = unpack("V", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"biasIndex"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 4;
        ${$lref}{"outputPointer"} = unpack("V", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"outputIndex"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 1;
        ${$lref}{"preOp"} = unpack("C", $buffer);
        read $inBlob_hnd, $buffer, 1;
        ${$lref}{"postOp"} = unpack("C", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"postParam1"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"postOpStrideX"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 2;
        ${$lref}{"postOpStrideY"} = unpack("v", $buffer);
        read $inBlob_hnd, $buffer, 2; #padding ???
        push(@layers, $lref);
    }
}

sub ExtractCoefficients
{
    $|++; #turn on autoflush
    foreach my $layer_ref (@layers)
    {
        my %layer = %$layer_ref;
        my %weights_data;
        my %bias_data;
        my $read_buff;
        my $ich = 0;
        my $och = 0;
        my $kx = 0;
        my $ky = 0;        

        if($layer{"stageType"} == 0 || $layer{"stageType"} == 4)
        {
            seek $inBlob_hnd, $weight_location + $layer{"weightsPointer"}, 0;            
            for($ky = 0; $ky < $layer{"radixY"}; $ky++)
            {
                for($kx = 0; $kx < $layer{"radixX"}; $kx++)
                {
                    for($ich = 0; $ich < $layer{"tapDimY"}; $ich++)                    
                    {
                        for($och = 0; $och < $layer{"tapDimZ"}; $och++)
                        {
                            read $inBlob_hnd, $read_buff, 2;
                            $weights_data{"i".$ich."o".$och."y".$ky."x".$kx} = $read_buff;
                        }
                    }
                }
            }

            if($layer{"postOp"} == 6 || $layer{"postOp"} == 9)
            {
                seek $inBlob_hnd, $weight_location + $layer{"biasPointer"}, 0;
                for($och = 0; $och < $layer{"tapDimZ"}; $och++)
                {
                    read $inBlob_hnd, $read_buff, 2;
                    $bias_data{"o".$och} = $read_buff;
                }
            }
            
            my $coeff_groups = $layer{"outputDimZ"} / 8;
            if(($layer{"outputDimZ"} % 8) != 0)
            {
                $coeff_groups++;
            }
            
            for(my $grp = 0; $grp < $coeff_groups; $grp++)
            {
                for($ich = 0; $ich < $layer{"tapDimY"}; $ich++)
                {
                    for($ky = 0; $ky < $layer{"radixY"}; $ky++)
                    {
                        for($kx = 0; $kx < $layer{"radixX"}; $kx++)
                        {
                            for($och = 0; $och < 8; $och++)
                            {
                                my $coeff = $weights_data{"i".$ich."o".($grp * 8 + $och)."y".$ky."x".$kx};
                                $coeff = defined $coeff ? $coeff : 0;
                                print $outBlob_hnd $coeff;
                            }
                        }
                    }
                }
            }

            if($layer{"postOp"} == 6 || $layer{"postOp"} == 9)
            {
                for($och = 0; $och < $layer{"tapDimZ"}; $och++)
                {
                    print $outBlob_hnd $bias_data{"o".$och};
                }
            }
        }
        print ".";
    }
    print "\n\n";
}

sub GetLayersOutputOffset
{
    my %LayerBaseAddress;
    foreach my $layer_ref(@layers)
    {
        my %layer = %$layer_ref;
        my $outIndex = $layer{"outputIndex"};
        my $outPointer = $layer{"outputPointer"};
        my $offset = 0;
        
        if(! exists $LayerBaseAddress{$outIndex})
        {
            $LayerBaseAddress{$outIndex} = $outPointer;
        }
        else
        {
            $offset = $outPointer - $LayerBaseAddress{$outIndex};
        }
        ${$layer_ref}{"OutputOffset"} = $offset;
    }
}

sub OptimizeTempBuffers
{    
    my @Buffers = (1, 1, 1);
    $Index2OptimizedIndex{1} = 1;
    $Index2OptimizedIndex{2} = 2;
    foreach my $layer_ref (@layers)
    {
        my %layer = %$layer_ref;

        my $inIndex = $layer{"inputIndex"};
        my $outIndex = $layer{"outputIndex"};
        my $newInIndex = 0;
        my $newOutIndex = 0;        

        $newInIndex = $Index2OptimizedIndex{$inIndex};

        if(exists $Index2OptimizedIndex{$outIndex})
        {
            $newOutIndex = $Index2OptimizedIndex{$outIndex};
        }
        else
        {
            #get a free buffer
            my $bufFound = 0;
            for($newOutIndex = 2; $newOutIndex < scalar @Buffers; $newOutIndex++)
            {
                if($Buffers[$newOutIndex] == 0 && ($newInIndex != $newOutIndex))
                {
                    $bufFound = 1;
                    last;
                }
            }
            if($bufFound == 0)
            {
                #create a new buffer
                push @Buffers, 1;
                $newOutIndex = $#Buffers;
            }                
            $Buffers[$newOutIndex] = 1;
            $Index2OptimizedIndex{$outIndex} = $newOutIndex;
        }
        $Buffers[$newInIndex] = 0;
        $Buffers[1] = 1;
        $Buffers[2] = 1;
    }
}

sub EliminateSoftMax
{
    my @newLayers;
    my %layerTypes = (0=>1, 1=>1, 2=>1, 4=>1);
    foreach my $layer_ref (@layers)
    {
        my %layer = %$layer_ref;
        if ($layerTypes{$layer{"stageType"}} != 1)
        {
            $Index2OptimizedIndex{$layer{"inputIndex"}} = $Index2OptimizedIndex{$layer{"outputIndex"}};
        }
        else
        {
            push @newLayers, $layer_ref;
        }
    }
    @layers = @newLayers;
}

sub GetFinalLayerCount
{
    my $count = 0;
    for(my $layer_index = 0; $layer_index < (scalar @layers); $layer_index++)
    {
        my %layer = %{$layers[$layer_index]};      
        if($layer{"stageType"} == 0) #convolution
        {
            $count++;
        }
        elsif(($layer{"stageType"}) == 1 || ($layer{"stageType"} == 2)) #pooling (max or average)
        {
            $count++;
        }
        elsif(($layer{"stageType"}) == 4) #fully connected
        {
            my $noVectors = $layer{"outputDimZ"};
            $count += int($noVectors / 256);
            if($noVectors % 256)
            {
                $count++;
            }            
        }        
    }
    return $count;
}

sub getMode
{
    my $inputSize = shift;
    my $mode = 0;
    if(0 == $inputSize % 8)
    {
        if(0 == $inputSize % 16)
        {
            $mode = 4;
        }
        elsif(0 == $inputSize % 8)
        {
            $mode = 3;
        }
        elsif(0 == $inputSize % 4)
        {
            $mode = 2;
        }
        elsif(0 == $inputSize % 2)
        {
            $mode = 1;
        }
    }
    return $mode;
}

sub WriteCNNStruct2Blob
{
    my @layer_types = ("convolution", "max_pooling", "average pooling", "softmax", "fully connected", "none", "relu", "reluX", "depth convolution", "bias", "prelu", "LRN");

    my $inputAddr = 0;
    my $outputAddr = 0;
    my $weightsAddr = GetFinalLayerCount() * 8 * 16;
    my $rsvd = 0;
    my $link_addr = 8 * 16;

    for(my $layer_index = 0; $layer_index < (scalar @layers); $layer_index++)
    {
        my %layer = %{$layers[$layer_index]};
               
        #calculate input strides
        my $iStrideX = 2;
        my $iStrideY = $layer{"inputDimX"} * $iStrideX;
        my $iStrideZ = $layer{"inputDimY"} * $iStrideY;

        #calculate output strides
        my $oStrideX = 2;
        my $oStrideY = $layer{"outputDimX"} * $oStrideX;
        my $oStrideZ = $layer{"outputDimY"} * $oStrideY;
        
        my $output_offset = ($layer{"OutputOffset"} / 2) * $oStrideZ;
        $inputAddr = $Index2OptimizedIndex{$layer{"inputIndex"}} << 24;
        $outputAddr = ($Index2OptimizedIndex{$layer{"outputIndex"}} << 24) + $output_offset;
           
        #calculate weights size
        my $weightsSize = 0;
        my $noOfGroups = $layer{"outputDimZ"} / 8;
        if($layer{"outputDimZ"} % 8 != 0)
        {
            $noOfGroups++;
        }
        $weightsSize = $layer{"tapDimX"} * $layer{"tapDimY"} * $noOfGroups * 8 * $iStrideX;        

        #padding
        my $padEn = 0;
        my $padType = 0x0F; #pad by repetition

        my $valid_out_x = ceil(($layer{"inputDimX"} - $layer{"radixX"} + 1) / $layer{"strideX"});
        my $valid_out_y = ceil(($layer{"inputDimY"} - $layer{"radixY"} + 1) / $layer{"strideY"});
        my $same_out_x = ceil($layer{"inputDimX"} / $layer{"strideX"});
        my $same_out_y = ceil($layer{"inputDimY"} / $layer{"strideY"});
        my $pad_along_x = ($layer{"outputDimX"} - 1) * $layer{"strideX"} + $layer{"radixX"} - $layer{"inputDimX"};
        my $pad_along_y = ($layer{"outputDimY"} - 1) * $layer{"strideY"} + $layer{"radixY"} - $layer{"inputDimY"};
        my $pad_left = int($pad_along_x / 2);
        my $pad_right = $pad_along_x - $pad_left;
        my $pad_top = int($pad_along_y / 2);
        my $pad_bottom = $pad_along_y - $pad_top;

        if(($layer{"outputDimX"} != $valid_out_x) || ($layer{"outputDimY"} != $valid_out_y))
        {
            $padEn = 1;
        }
        
        #relu reluX
        my $reluEn = 0;
        my $reluXEn = 0;
        my $A1 = 0;
        my $A0 = 0;
        my $T0 = 0;
        my $X = 0;

        #bias & scale
        my $biasAddr = 0;
        my $biasSize = 0;
        my $scaleAddr = 0; #currently not used by the Fathom blob
        my $scaleSize = 0; #currently not used by the Fathom blob

        if($layer{"postOp"} == 9)
        {
            $biasSize = $layer{"outputDimZ"} * 2;
            $biasAddr = $weightsAddr + $weightsSize;
        }
        if($layer{"postOp"} == 6) #Relu
        {
            $biasSize = $layer{"outputDimZ"} * 2;
            $biasAddr = $weightsAddr + $weightsSize;
            $reluEn = 1; #use Relu for this post operation
            $A0 = 0;
            $A1 = 1;
            $T0 = 0;
            $X = $layer{"postParam1"};
        }        
        
        if($layer{"stageType"} == 0) #convolution
        {
            if($layer_index == $#layers)
            {
                $link_addr = 0; #last layer has link address set to null
            }

            #Calculate mode (based on the number of input channels)
            my $mode = getMode($layer{"inputDimZ"});
            my $blocks = 1 << $mode;
            my $chansPerBlock = int($layer{"inputDimZ"} / $blocks);

            my $coeff_LPB = $chansPerBlock * $layer{"radixX"} * $layer{"radixY"};
            if($coeff_LPB > 256)
            {
                die "Cannot convert blob file. Convolution stage ".$layer{"stageName"}." has an unsupported number of input channels\n";
            }

            #calculate weights strides
            my $wStrideIn = $layer{"radixX"} * $layer{"radixY"} * 8 * $iStrideX;
            my $wStrideOut = $layer{"inputDimZ"} * $wStrideIn;

            $padType = 0x00; #pad by zero
            
            my $css = $layer{"radixX"} * $layer{"radixY"};
            #Create convolution layer structure
            my $layer_struct .= push_bits([11,$rsvd], [1, 0], [4,$rsvd], [4, 0], [4, 0x0A], [2,$rsvd], [3, $mode], [3, 0], [32, $link_addr]);
            $layer_struct .= push_bits([5,$rsvd], [11, $layer{"outputDimZ"}-1], [5,$rsvd], [11, $layer{"inputDimZ"}-1], [4,$rsvd], [12, $layer{"inputDimX"}-1], [4,$rsvd], [12, $layer{"inputDimY"}-1]);
            $layer_struct .= push_bits([1, $padEn], [4, $padType], [19,$rsvd], [4, $layer{"radixX"}-1], [4, $layer{"radixY"}-1], [12,$rsvd], [4, $layer{"strideX"}-1], [5,$rsvd], [11, $chansPerBlock-1]);
            $layer_struct .= push_bits([1, 0], [15,$rsvd], [16, $X], [8, 0], [8, 0], [15,$rsvd], [1, 0]); #pool disabled for convolution
            $layer_struct .= push_bits([1, $reluEn], [1, $reluXEn], [10, $A1], [10, $A0], [10, $T0], [32, $inputAddr]);
            $layer_struct .= push_bits([32, $iStrideY], [32, $iStrideZ]);
            $layer_struct .= push_bits([32, $wStrideOut], [32, $weightsAddr]);
            $layer_struct .= push_bits([32, $oStrideY], [32, $wStrideIn]);
            $layer_struct .= push_bits([32, $oStrideZ], [32, $outputAddr]);
            $layer_struct .= push_bits([1, 0], [22,$rsvd], [9, 0], [3,$rsvd], [13, 0], [7,$rsvd], [9, 0]);
            $layer_struct .= push_bits([4,$rsvd], [12, $layer{"outputDimX"}], [8, $css - 1], [8, $coeff_LPB - 1], [23,$rsvd], [9, 0]);
            $layer_struct .= push_bits([32, $scaleAddr], [32,$biasAddr]);
            $layer_struct .= push_bits([16, 0], [16, 0], [16, 0], [16, 0]);
            $layer_struct .= push_bits([16, 0], [16, 0], [16, 0], [16, 0]);
            $layer_struct .= push_bits([16, 0], [16, 0], [16, 0], [16, 0]);
            $layer_struct .= push_bits([16, 0], [16, 0], [16, 0], [16, 0]);
            print $outBlob_hnd get_struct($layer_struct); 
            $weightsAddr += $weightsSize + $biasSize;
            $link_addr += 8 * 16;            
        }
        elsif(($layer{"stageType"}) == 1 || ($layer{"stageType"} == 2)) #pooling (max or average)
        {
            if($layer_index == $#layers)
            {
                $link_addr = 0; #last layer has link address set to null
            }
            $padType = 0x00;
            my $poolType = $layer{"stageType"} - 1; #0-max; 1-average
            if ($poolType == 0)
            {
                if($pad_left)
                {
                    $padType |= 0x08;
                }
                if($pad_right)
                {
                    $padType |= 0x01;
                }
                if($pad_top)
                {
                    $padType |= 0x04;
                }
                if($pad_bottom)
                {
                    $padType |= 0x02;
                }                
            }
            else
            {
                $padType = 0x00; #pad by zero
                #compute the X value (1/radixX * radixY) - use float16tohex to do it
                my $Xf = 1 / ($layer{"radixX"} * $layer{"radixY"});
                `echo $Xf | ./float16tohex` =~ /0x(\w+)|$/;
                $X = hex($1);
            }
            #Create pooling layer structure
            my $layer_struct .= push_bits([11,$rsvd], [1, 0], [4,$rsvd], [4, 0], [4, 0x0B], [2,$rsvd], [3, 0], [3, 4], [32, $link_addr]);
            $layer_struct .= push_bits([5,$rsvd], [11, $layer{"outputDimZ"}-1], [5,$rsvd], [11, $layer{"inputDimZ"}-1], [4,$rsvd], [12, $layer{"inputDimX"}-1], [4,$rsvd], [12, $layer{"inputDimY"}-1]);
            $layer_struct .= push_bits([1, $padEn], [4, $padType], [27,$rsvd], [12,$rsvd], [4, $layer{"strideX"}-1], [5,$rsvd], [11, $layer{"inputDimZ"}-1]);
            $layer_struct .= push_bits([1, $poolType], [15,$rsvd], [16, $X], [8, $layer{"radixX"}-1], [8, $layer{"radixY"}-1], [15,$rsvd], [1, 1]);
            $layer_struct .= push_bits([1, $reluEn], [1, $reluXEn], [10, $A1], [10, $A0], [10, $T0], [32, $inputAddr]);
            $layer_struct .= push_bits([32, $iStrideY], [32, $iStrideZ]);
            $layer_struct .= push_bits([32, $rsvd], [32,$rsvd]);
            $layer_struct .= push_bits([32, $oStrideY], [32,$rsvd]);
            $layer_struct .= push_bits([32, $oStrideZ], [32, $outputAddr]);
            $layer_struct .= push_bits([1, 0], [22,$rsvd], [9, 0], [3,$rsvd], [13, 0], [7,$rsvd], [9, 0]);
            $layer_struct .= push_bits([4,$rsvd], [12, $layer{"outputDimX"}], [16, $rsvd], [23,$rsvd], [9, 0]);
            $layer_struct .= push_bits([32, $scaleAddr], [32,$biasAddr]);
            $layer_struct .= push_bits([16, 0], [16, 0], [16, 0], [16, 0]);
            $layer_struct .= push_bits([16, 0], [16, 0], [16, 0], [16, 0]);
            $layer_struct .= push_bits([16, 0], [16, 0], [16, 0], [16, 0]);
            $layer_struct .= push_bits([16, 0], [16, 0], [16, 0], [16, 0]);
            print $outBlob_hnd get_struct($layer_struct); 
            $weightsAddr += $biasSize;
            $link_addr += 8 * 16;
        }
        elsif(($layer{"stageType"}) == 4) #fully connected
        {
            
            my $inputWidth = $layer{"inputDimZ"};

            my $Blocks = int($inputWidth / 256);
            if($inputWidth % 256)
            {
                $Blocks++;
            }
            my %BlockIntervals = (1=>1,   2=>2,  3=>4,4=>4,   5=>8,6=>8,7=>8,8=>8,   9=>16,10=>16,11=>16,12=>16,13=>16,14=>16,15=>16,16=>16);
            my %BlocksToMode = (1=>0, 2=>1, 4=>2, 8=>3, 16=>4);
            if(! exists $BlockIntervals{$Blocks})
            {
                die "Cannot convert blob file. Fully connected stage ".$layer{"stageName"}." has an unsupported vector length\n";
            }
            $Blocks = $BlockIntervals{$Blocks};

            if($inputWidth % $Blocks != 0)
            {
                die "Cannot convert blob file. Fully connected stage ".$layer{"stageName"}." has an unsupported vector length\n";
            }            

            my $inputLineStride = 2;
            my $vectorLPB = ($inputWidth / $Blocks);
            my $inputBlockStride = $vectorLPB * $inputLineStride;
            my $dataPerRamBlock = $vectorLPB; #$inputBlockStride;
            
            my $outputLineStride = 2;
            my $noVectors = $layer{"outputDimZ"};
            my $vStrideIn = $inputBlockStride * 8;
            my $vStrideOut = $vStrideIn * $Blocks;

            my $remVectors = $noVectors;
            my $lindex = 0;
            while($remVectors)
            {                
                my $vectorsThisTime = $remVectors;
                if($vectorsThisTime > 256)
                {
                    $vectorsThisTime = 256;
                }
                $remVectors -= $vectorsThisTime;
                if(($layer_index == $#layers) && ($remVectors == 0))
                {
                    $link_addr = 0; #last layer has link address set to null
                }
                #Create fully connected layer structure
                my $layer_struct .= push_bits([11,$rsvd], [1, 0], [4,$rsvd], [4, 0], [4, 0xC], [2,$rsvd], [3, $BlocksToMode{$Blocks}], [3, 2], [32, $link_addr]);
                $layer_struct .= push_bits([24,$rsvd], [8, $vectorsThisTime-1], [20,$rsvd], [12, $inputWidth-1]);
                $layer_struct .= push_bits([32,$rsvd], [23, $rsvd], [9, $dataPerRamBlock - 1]);
                $layer_struct .= push_bits([16,$rsvd], [16, $X], [32,$rsvd]);
                $layer_struct .= push_bits([1, $reluEn], [1, $reluXEn], [10, $A1], [10, $A0], [10, $T0], [32, $inputAddr]);
                $layer_struct .= push_bits([32, $inputLineStride], [32, $inputBlockStride]);
                $layer_struct .= push_bits([32, $vStrideOut], [32, $weightsAddr + $lindex * $vStrideOut * 32]);
                $layer_struct .= push_bits([32, $outputLineStride], [32, $vStrideIn]);
                $layer_struct .= push_bits([32, 2], [32, $outputAddr + $lindex * 256 * $outputLineStride]);
                $layer_struct .= push_bits([1, 0], [31,$rsvd], [3,$rsvd], [13, 0], [7,$rsvd], [9, 0]);
                $layer_struct .= push_bits([24,$rsvd], [8, $vectorLPB - 1], [15,$rsvd], [1, 0], [16,$rsvd]);
                $layer_struct .= push_bits([32, $scaleAddr], [32,$biasAddr + $lindex * 256 * $inputLineStride]);
                $layer_struct .= push_bits([16, 0], [16, 0], [16, 0], [16, 0]);
                $layer_struct .= push_bits([16, 0], [16, 0], [16, 0], [16, 0]);
                $layer_struct .= push_bits([16, 0], [16, 0], [16, 0], [16, 0]);
                $layer_struct .= push_bits([16, 0], [16, 0], [16, 0], [16, 0]);
                print $outBlob_hnd get_struct($layer_struct);
                $lindex++;
                $link_addr += 8 * 16;
            }
            $weightsAddr += $weightsSize + $biasSize;
        }        
    }
}

sub push_bits
{
	my $ret;
	foreach my $p (@_)
	{
		my @param = @$p;
        for(my $index = 0; $index < $param[0]; $index++)
        {
            $ret .= ($param[1] >> ($param[0] - $index - 1)) & 1;
        }
	}
	return $ret;
}
sub get_struct
{
	my $struct = shift;
	my $ret;
	die "Length of struct not multiple of 64bit\n" if((length $struct) % 64 != 0);
	for(my $index = 0; $index < (length $struct) / 64; $index++)
	{
		my $bytestr = substr $struct, $index * 64, 64;
        my $line;
        for(my $byteindex = 0; $byteindex < 8; $byteindex++)
        {
            my $byte = 0;
            for(my $bitindex = 0; $bitindex < 8; $bitindex++)
            {
                my $bit = 0;
                $bit = 1 if (substr( $bytestr, $byteindex * 8 + $bitindex, 1) eq "1");			
                $byte <<= 1;
                $byte |= $bit;
            }
            $line = pack("C", $byte).$line;            
        }
        $ret .= $line;
	}
	return $ret;
}