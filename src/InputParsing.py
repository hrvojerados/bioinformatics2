import random

input_file = "BIOINF_INPUT.fasta"
output_file = "Train_set.txt"

def main():
    f = open(input_file, "r")
    lines = f.read().split('>')[1:]
    train= lines[:len(lines)//2]
    test= lines[len(lines)//2:]
    pairs = list(make_pairs(train, 2000))
    f.close()
    out = open(output_file, "w")
    for pair in pairs:
        out.write(connect_holes(clear_empty(pair))[0]+'\n'+connect_holes(clear_empty(pair))[1]+'\n')
        out.write(">>>\n")
    out.close()
def make_pairs(lines, n):
    
    pairs = set()
    while len(pairs) < n:
        pair = (random.sample(lines,2))
        pairs.add((pair[0].split('\n',1)[1].replace('\n','')+'\n',pair[1].split('\n',1)[1].replace('\n','')+'\n'))
    return pairs
        
def clear_empty(pair):
    seq1=pair[0]
    seq2=pair[1]
    newseq1=''
    newseq2=''
    i1 = 0
    i2 = 0
    while (seq1[i1] != '\n' and seq2[i2]!= '\n'):
        c1,c2=seq1[i1],seq2[i2]
        if c1 !='-' or c2!='-':
            newseq1+=c1
            newseq2+=c2
        i1,i2=i1+1,i2+1
    return [newseq1+'\n', newseq2+'\n']

def connect_holes(pair):
    seq1=pair[0]
    seq2=pair[1]
    newseq1, newseq2='',''
    i1, i2 = 0,0
    empty1, empty2 = 0,0
    while (seq1[i1] != '\n' and seq2[i2]!= '\n'):
        c1,c2=seq1[i1],seq2[i2]
        if (c1 != '-' and c2 != '-'):
            newseq1 = newseq1+'-'*empty1
            newseq2 = newseq2+'-'*empty2
            empty1, empty2 = 0,0
            newseq1,newseq2 = newseq1+c1, newseq2+c2
        elif c1 =='-':
            empty1 = empty1+1
            newseq2 = newseq2+c2
        else:
            empty2 = empty2+1
            newseq1 = newseq1+c1
        while (empty1>0 and empty2>0):
            empty1, empty2 = empty1-1, empty2-1
        i1,i2=i1+1,i2+1
    newseq1 = newseq1+'-'*empty1
    newseq2 = newseq2+'-'*empty2
    return [newseq1, newseq2]


if __name__=='__main__':
    main()
