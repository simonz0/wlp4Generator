.import new
.import delete
.import print
lis $4 ; $4 will always hold 4
.word 4
lis $10 ; $10 will always hold address for print
.word print
lis $11 ; $11 will always hold 1
.word 1
.import init
sw $31, -4($30)
sub $30, $30, $4
lis $5
.word init
jalr $5
add $30 , $30 , $4
lw $31, -4( $30 )
beq $0, $0, GENERATORwain
GENERATORfun:
sub $29 , $30 , $4 ; assume caller saved old frame procedure
add $3 , $0 , $11
sw $3, -4($30)
sub $30, $30, $4
add $3 , $0 , $11
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 1
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 2
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 0
sw $3 , -8( $29 )
loop289383:
lw $3 , -8( $29 )
sw $3, -4($30)
sub $30, $30, $4
lw $3 , 4( $29 )
add $30 , $30 , $4
lw $5, -4( $30 )
slt $3 ,$5 ,$3
beq $3 , $0 ,endWhile289383
sw $1, -4($30)
sub $30, $30, $4
lw $3 , 8( $29 )
sw $3, -4($30)
sub $30, $30, $4
lw $3 , -8( $29 )
mult $3 , $4 ; $4 always has the value 4
mflo $3
add $30 , $30 , $4
lw $5, -4( $30 )
add $3 , $5 , $3
lw $3 , 0( $3 ) ; $3 contains the loaded value
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 10
add $30 , $30 , $4
lw $5, -4( $30 )
add $3, $5, $3
add $1 , $3 , $0
sw $31, -4($30)
sub $30, $30, $4
lis $5
.word print
jalr $5
add $30 , $30 , $4
lw $31, -4( $30 )
add $30 , $30 , $4
lw $1, -4( $30 )
lw $3 , -8( $29 )
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 1
add $30 , $30 , $4
lw $5, -4( $30 )
add $3, $5, $3
sw $3 , -8( $29 )
beq $0 , $0 , loop289383
endWhile289383:
lis $3
.word 0
add $30 ,$29 ,$4 ;reset stackframe
jr $31
GENERATORfoo:
sub $29 , $30 , $4 ; assume caller saved old frame procedure
add $3 , $0 , $11
sw $3, -4($30)
sub $30, $30, $4
add $3 , $0 , $11
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 1
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 2
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 0
sw $3 , -8( $29 )
loop930886:
lw $3 , -8( $29 )
sw $3, -4($30)
sub $30, $30, $4
lw $3 , 4( $29 )
add $30 , $30 , $4
lw $5, -4( $30 )
slt $3 ,$5 ,$3
beq $3 , $0 ,endWhile930886
sw $1, -4($30)
sub $30, $30, $4
lw $3 , 8( $29 )
sw $3, -4($30)
sub $30, $30, $4
lw $3 , -8( $29 )
mult $3 , $4 ; $4 always has the value 4
mflo $3
add $30 , $30 , $4
lw $5, -4( $30 )
add $3 , $5 , $3
lw $3 , 0( $3 ) ; $3 contains the loaded value
add $1 , $3 , $0
sw $31, -4($30)
sub $30, $30, $4
lis $5
.word print
jalr $5
add $30 , $30 , $4
lw $31, -4( $30 )
add $30 , $30 , $4
lw $1, -4( $30 )
lw $3 , -8( $29 )
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 1
add $30 , $30 , $4
lw $5, -4( $30 )
add $3, $5, $3
sw $3 , -8( $29 )
beq $0 , $0 , loop930886
endWhile930886:
sw $29, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lw $3 , 8( $29 )
sw $3, -4($30)
sub $30, $30, $4
lw $3 , 4( $29 )
sw $3, -4($30)
sub $30, $30, $4
lis $5
.word GENERATORfun
jalr $5
add $30 , $30 , $4
lw $7, -4( $30 )
add $30 , $30 , $4
lw $7, -4( $30 )
add $30 , $30 , $4
lw $31, -4( $30 )
add $30 , $30 , $4
lw $29, -4( $30 )
sw $3 , -12( $29 )
lis $3
.word 0
add $30 ,$29 ,$4 ;reset stackframe
jr $31
GENERATORwain:
; begin Prologue
sub $29 , $30 , $4 ; setup frame pointer
;end Prologue and begin Body

sw $1, -4($30)
sub $30, $30, $4
sw $2, -4($30)
sub $30, $30, $4
lis $3
.word 0
sw $3, -4($30)
sub $30, $30, $4
sw $29, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lw $3 , 0( $29 )
sw $3, -4($30)
sub $30, $30, $4
lw $3 , -4( $29 )
sw $3, -4($30)
sub $30, $30, $4
lis $5
.word GENERATORfoo
jalr $5
add $30 , $30 , $4
lw $7, -4( $30 )
add $30 , $30 , $4
lw $7, -4( $30 )
add $30 , $30 , $4
lw $31, -4( $30 )
add $30 , $30 , $4
lw $29, -4( $30 )
sw $3 , -8( $29 )
lw $3 , -8( $29 )
jr $31
