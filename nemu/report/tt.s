
	00 
	00 00 
	00 00 00 
	01 00 00 00 
	01 c0                	add    %eax,%eax
	01 c1                	add    %eax,%ecx
	01 c2                	add    %eax,%edx
	01 ca                	add    %ecx,%edx
	01 cf                	add    %ecx,%edi
	01 d0                	add    %edx,%eax
	01 d6                	add    %edx,%esi
	01 d9                	add    %ebx,%ecx
	01 df                	add    %ebx,%edi
	01 f2                	add    %esi,%edx
	01 fa                	add    %edi,%edx
	01 fe                	add    %edi,%esi
	03 04 9d 7c 01 10 00 	add    0x10017c(,%ebx,4),%eax
	03 04 dd 20 03 10 00 	add    0x100320(,%ebx,8),%eax
	03 45 08             	add    0x8(%ebp),%eax
	03 55 08             	add    0x8(%ebp),%edx
	09 c1                	or     %eax,%ecx
	09 d0                	or     %edx,%eax
	0a 02                	or     (%edx),%al
	0f 85 6f ff ff ff    	jne    10007b <main+0x1b>
	0f 8d 90 00 00 00    	jge    1000fd <partition+0xb5>
	0f 94 c0             	sete   %al
	0f 94 c2             	sete   %dl
	0f 95 c0             	setne  %al
	0f af 03             	imul   (%ebx),%eax
	0f af c2             	imul   %edx,%eax
	0f af c6             	imul   %esi,%eax
	0f af c7             	imul   %edi,%eax
	0f af c8             	imul   %eax,%ecx
	0f af d0             	imul   %eax,%edx
	0f af d7             	imul   %edi,%edx
	0f af da             	imul   %edx,%ebx
	0f af f0             	imul   %eax,%esi
	0f af f8             	imul   %eax,%edi
	0f af fa             	imul   %edx,%edi
	0f b6 c0             	movzbl %al,%eax
	0f b6 d2             	movzbl %dl,%edx
	0f b7 84 1b 00 02 10 	movzwl 0x100200(%ebx,%ebx,1),%eax
	0f b7 c0             	movzwl %ax,%eax
	0f bf 84 1b 00 02 10 	movswl 0x100200(%ebx,%ebx,1),%eax
	13 14 dd 24 03 10 00 	adc    0x100324(,%ebx,8),%edx
	13 55 0c             	adc    0xc(%ebp),%edx
	1b 14 dd 24 03 10 00 	sbb    0x100324(,%ebx,8),%edx
	1b 55 14             	sbb    0x14(%ebp),%edx
	22 02                	and    (%edx),%al
	29 c8                	sub    %ecx,%eax
	29 ce                	sub    %ecx,%esi
	29 f0                	sub    %esi,%eax
	2b 04 dd 20 03 10 00 	sub    0x100320(,%ebx,8),%eax
	2b 45 10             	sub    0x10(%ebp),%eax
	31 c0                	xor    %eax,%eax
	31 c1                	xor    %eax,%ecx
	31 c9                	xor    %ecx,%ecx
	31 ca                	xor    %ecx,%edx
	31 d0                	xor    %edx,%eax
	31 db                	xor    %ebx,%ebx
	31 f6                	xor    %esi,%esi
	31 ff                	xor    %edi,%edi
	38 83 e0 00 10 00    	cmp    %al,0x1000e0(%ebx)
	39 04 9d 3c 01 10 00 	cmp    %eax,0x10013c(,%ebx,4)
	39 04 9d 40 01 10 00 	cmp    %eax,0x100140(,%ebx,4)
	39 04 9d 98 e3 0f 00 	cmp    %eax,0xfe398(,%ebx,4)
	39 04 9d a0 01 10 00 	cmp    %eax,0x1001a0(,%ebx,4)
	39 04 9d e0 00 10 00 	cmp    %eax,0x1000e0(,%ebx,4)
	39 05 00 02 10 00    	cmp    %eax,0x100200
	39 05 01 02 10 00    	cmp    %eax,0x100201
	39 05 04 02 10 00    	cmp    %eax,0x100204
	39 05 05 02 10 00    	cmp    %eax,0x100205
	39 05 09 02 10 00    	cmp    %eax,0x100209
	39 05 18 02 10 00    	cmp    %eax,0x100218
	39 05 fc 01 10 00    	cmp    %eax,0x1001fc
	39 14 9d 00 01 10 00 	cmp    %edx,0x100100(,%ebx,4)
	39 1c 9d 00 01 10 00 	cmp    %ebx,0x100100(,%ebx,4)
	39 1c 9d 20 02 10 00 	cmp    %ebx,0x100220(,%ebx,4)
	39 1c 9d a0 01 10 00 	cmp    %ebx,0x1001a0(,%ebx,4)
	39 1c 9d c0 01 10 00 	cmp    %ebx,0x1001c0(,%ebx,4)
	39 1c b5 e0 00 10 00 	cmp    %ebx,0x1000e0(,%esi,4)
	39 1c bd 04 01 10 00 	cmp    %ebx,0x100104(,%edi,4)
	39 1c bd 30 01 10 00 	cmp    %ebx,0x100130(,%edi,4)
	39 55 f0             	cmp    %edx,-0x10(%ebp)
	39 84 36 9e 01 10 00 	cmp    %eax,0x10019e(%esi,%esi,1)
	39 8f 20 01 10 00    	cmp    %ecx,0x100120(%edi)
	39 c1                	cmp    %eax,%ecx
	39 c3                	cmp    %eax,%ebx
	39 c8                	cmp    %ecx,%eax
	39 ca                	cmp    %ecx,%edx
	39 cb                	cmp    %ecx,%ebx
	39 ce                	cmp    %ecx,%esi
	39 cf                	cmp    %ecx,%edi
	39 d1                	cmp    %edx,%ecx
	39 d8                	cmp    %ebx,%eax
	39 d9                	cmp    %ebx,%ecx
	39 da                	cmp    %ebx,%edx
	39 de                	cmp    %ebx,%esi
	39 df                	cmp    %ebx,%edi
	39 f3                	cmp    %esi,%ebx
	3b 04 9d 10 01 10 00 	cmp    0x100110(,%ebx,4),%eax
	3b 04 9d 30 01 10 00 	cmp    0x100130(,%ebx,4),%eax
	3b 04 9d 60 01 10 00 	cmp    0x100160(,%ebx,4),%eax
	3b 04 9d c0 01 10 00 	cmp    0x1001c0(,%ebx,4),%eax
	3b 04 9d e0 01 10 00 	cmp    0x1001e0(,%ebx,4),%eax
	3b 45 08             	cmp    0x8(%ebp),%eax
	3b 45 0c             	cmp    0xc(%ebp),%eax
	3b 45 10             	cmp    0x10(%ebp),%eax
	3b 84 be 60 01 10 00 	cmp    0x100160(%esi,%edi,4),%eax
	3b 94 bb 00 01 10 00 	cmp    0x100100(%ebx,%edi,4),%edx
	3b 94 be 20 01 10 00 	cmp    0x100120(%esi,%edi,4),%edx
	3c 19                	cmp    $0x19,%al
	3d 2c 01 00 00       	cmp    $0x12c,%eax
	3d ba 13 00 00       	cmp    $0x13ba,%eax
	3d dd cc bb aa       	cmp    $0xaabbccdd,%eax
	3d f0 05 10 00       	cmp    $0x1005f0,%eax
	3d f4 01 00 00       	cmp    $0x1f4,%eax
	40                   	inc    %eax
	41                   	inc    %ecx
	43                   	inc    %ebx
	46                   	inc    %esi
	47                   	inc    %edi
	48                   	dec    %eax
	4a                   	dec    %edx
	4b                   	dec    %ebx
	50                   	push   %eax
	51                   	push   %ecx
	52                   	push   %edx
	53                   	push   %ebx
	55                   	push   %ebp
	56                   	push   %esi
	57                   	push   %edi
	58                   	pop    %eax
	59                   	pop    %ecx
	5a                   	pop    %edx
	5b                   	pop    %ebx
	5d                   	pop    %ebp
	5e                   	pop    %esi
	5f                   	pop    %edi
	66 89 86 ff 01 10 00 	mov    %ax,0x1001ff(%esi)
	66 90                	xchg   %ax,%ax
	66 c7 05 c0 00 10 00 	movw   $0x0,0x1000c0
	66 c7 45 f6 aa 00    	movw   $0xaa,-0xa(%ebp)
	68 1b 01 10 00       	push   $0x10011b
	68 20 02 10 00       	push   $0x100220
	68 23 38 00 00       	push   $0x3823
	68 29 01 10 00       	push   $0x100129
	68 2c 01 10 00       	push   $0x10012c
	68 2c 02 10 00       	push   $0x10022c
	68 3a 01 10 00       	push   $0x10013a
	68 45 01 10 00       	push   $0x100145
	68 4c 02 10 00       	push   $0x10024c
	68 80 01 10 00       	push   $0x100180
	6a 00                	push   $0x0
	6a 01                	push   $0x1
	6a 02                	push   $0x2
	6a 03                	push   $0x3
	6a 04                	push   $0x4
	6a 05                	push   $0x5
	6a 06                	push   $0x6
	6a 07                	push   $0x7
	6a 08                	push   $0x8
	6a 09                	push   $0x9
	6a 0a                	push   $0xa
	6a 0b                	push   $0xb
	6a 0c                	push   $0xc
	6a 0d                	push   $0xd
	6a 0e                	push   $0xe
	6a 0f                	push   $0xf
	6a 13                	push   $0x13
	6a 23                	push   $0x23
	74 02                	je     10003b <nemu_assert+0xf>
	74 02                	je     10015f <nemu_assert+0xf>
	74 08                	je     100068 <is_leap_year+0x20>
	74 08                	je     1000b4 <main+0x6c>
	74 0a                	je     1000dc <main+0x74>
	74 0b                	je     10007c <is_prime+0x34>
	74 0d                	je     1000ac <main+0x48>
	74 0f                	je     1000fc <goldbach+0x70>
	74 10                	je     1000a5 <main+0x45>
	74 13                	je     1000e7 <main+0x6b>
	74 15                	je     100076 <is_prime+0x2e>
	74 1a                	je     1000b4 <main+0x50>
	74 1a                	je     1000bb <main+0x57>
	74 1b                	je     100114 <main+0xac>
	74 1d                	je     1000c2 <goldbach+0x36>
	74 1e                	je     100093 <main+0x4b>
	74 21                	je     1000c7 <main+0x63>
	74 22                	je     1000fc <goldbach+0x70>
	74 2a                	je     100084 <is_prime+0x3c>
	74 2c                	je     1000d9 <main+0x5d>
	74 2d                	je     100100 <main+0x74>
	74 2e                	je     10009e <main+0x56>
	74 30                	je     1000c4 <main+0x7c>
	74 3f                	je     1000fc <goldbach+0x70>
	74 44                	je     100114 <goldbach+0x88>
	74 52                	je     1000fc <goldbach+0x70>
	74 6f                	je     1000f0 <main+0x98>
	74 75                	je     100101 <main+0x99>
	74 cb                	je     1000a4 <main+0x28>
	75 02                	jne    100089 <main+0x41>
	75 0d                	jne    10009c <setbit+0x34>
	75 17                	jne    100068 <is_leap_year+0x20>
	75 80                	jne    100063 <main+0x1b>
	75 8b                	jne    10008c <main+0x24>
	75 91                	jne    100081 <main+0x29>
	75 95                	jne    10007c <main+0x20>
	75 9f                	jne    1000a0 <goldbach+0x14>
	75 a4                	jne    100094 <main+0x34>
	75 ac                	jne    100074 <main+0x2c>
	75 ad                	jne    100078 <main+0x20>
	75 b0                	jne    100078 <main+0x20>
	75 b1                	jne    100074 <main+0x20>
	75 bd                	jne    100094 <main+0x38>
	75 c6                	jne    100050 <select_sort+0x8>
	75 c6                	jne    100064 <main+0x1c>
	75 c6                	jne    1000a8 <main+0x18>
	75 c6                	jne    100114 <main+0x84>
	75 c9                	jne    10005c <main+0x14>
	75 cc                	jne    10005c <main+0x14>
	75 cd                	jne    100054 <bubble_sort+0xc>
	75 cd                	jne    1000a0 <main+0x14>
	75 cd                	jne    100108 <main+0x7c>
	75 cd                	jne    10010c <main+0xc4>
	75 d2                	jne    100070 <main+0x28>
	75 d4                	jne    100084 <main+0x2c>
	75 d4                	jne    1000a9 <main+0x49>
	75 d6                	jne    10008c <main+0x44>
	75 d6                	jne    1000bc <main+0x74>
	75 d7                	jne    10005c <main+0x14>
	75 d8                	jne    100080 <main+0x2c>
	75 d9                	jne    100060 <main+0x18>
	75 d9                	jne    10008c <main+0x44>
	75 da                	jne    1000b8 <main+0x70>
	75 de                	jne    10006c <main+0x24>
	75 de                	jne    100090 <main+0x48>
	75 df                	jne    100138 <main+0x14>
	75 e2                	jne    1000b4 <main+0x6c>
	75 e2                	jne    1000d8 <main+0x4c>
	75 e2                	jne    1000e4 <main+0x54>
	75 e2                	jne    100140 <main+0xb4>
	75 e2                	jne    100150 <main+0xc0>
	75 e2                	jne    100178 <main+0x24>
	75 e2                	jne    1001b8 <main+0x64>
	75 e9                	jne    100084 <main+0x3c>
	75 eb                	jne    100060 <select_sort+0x18>
	75 eb                	jne    1000b8 <main+0x28>
	75 eb                	jne    100124 <main+0x94>
	75 ec                	jne    10007c <main+0x34>
	75 ef                	jne    100080 <main+0x38>
	75 f1                	jne    1000d8 <partition+0x90>
	75 f2                	jne    100068 <is_prime+0x20>
	75 f2                	jne    100068 <main+0x20>
	75 f2                	jne    10006c <main+0x24>
	75 f2                	jne    1000b4 <goldbach+0x28>
	75 f2                	jne    1000e4 <goldbach+0x58>
	75 f3                	jne    10005c <main+0x14>
	75 f7                	jne    100058 <fact+0x10>
	75 f7                	jne    100078 <main+0x30>
	75 f7                	jne    10009c <main+0x54>
	75 f7                	jne    1000b0 <main+0x4c>
	76 04                	jbe    1000a0 <main+0x40>
	76 09                	jbe    100061 <fact+0x19>
	77 06                	ja     10005c <to_lower_case+0x14>
	77 09                	ja     10005c <switch_case+0x14>
	7c 0c                	jl     1000ec <partition+0xa4>
	7c da                	jl     100126 <quick_sort+0x16>
	7c db                	jl     10005c <bubble_sort+0x14>
	7c db                	jl     1000a8 <main+0x1c>
	7c db                	jl     100110 <main+0x84>
	7d 02                	jge    100094 <main+0x3c>
	7d 03                	jge    100056 <max+0xe>
	7d 04                	jge    10006f <select_sort+0x27>
	7d 04                	jge    1000c7 <main+0x37>
	7d 04                	jge    100133 <main+0xa3>
	7d 05                	jge    100042 <f0+0x16>
	7d 05                	jge    100086 <f1+0x16>
	7d 05                	jge    1000c1 <f2+0x19>
	7d 05                	jge    100104 <f3+0x1c>
	7d 25                	jge    100081 <bubble_sort+0x39>
	7d 25                	jge    1000cd <main+0x41>
	7d 25                	jge    100135 <main+0xa9>
	7d 26                	jge    10014c <quick_sort+0x3c>
	7d 2e                	jge    1000f6 <partition+0xae>
	7d e9                	jge    1000a4 <main+0x18>
	7e 02                	jle    1000a7 <main+0x47>
	7e 02                	jle    1000b9 <main+0x59>
	7e 03                	jle    100056 <min3+0xe>
	7e 03                	jle    10005e <min3+0x16>
	7e 10                	jle    1000a0 <f1+0x30>
	7e 15                	jle    1000e0 <f2+0x38>
	7e 1c                	jle    100068 <f0+0x3c>
	7e 27                	jle    10007c <is_prime+0x34>
	7e 30                	jle    1000b0 <partition+0x68>
	7e 31                	jle    1000fc <goldbach+0x70>
	7e 32                	jle    100140 <f3+0x58>
	7e 61                	jle    100101 <goldbach+0x75>
	7e bf                	jle    1000a0 <main+0x14>
	7e e9                	jle    100058 <bubble_sort+0x10>
	7e e9                	jle    10010c <main+0x80>
	7f 10                	jg     10006c <if_else+0x24>
	7f 16                	jg     10007c <if_else+0x34>
	7f 1f                	jg     100074 <if_else+0x2c>
	7f 23                	jg     100084 <if_else+0x3c>
	7f 41                	jg     1000e8 <main+0x5c>
	7f 44                	jg     1000f0 <main+0x64>
	7f 47                	jg     1000f8 <main+0x6c>
	7f f0                	jg     100078 <partition+0x30>
	80 7d 10 00          	cmpb   $0x0,0x10(%ebp)
	80 7d f7 55          	cmpb   $0x55,-0x9(%ebp)
	80 f9 19             	cmp    $0x19,%cl
	81 ef 60 04 10 00    	sub    $0x100460,%edi
	81 fb 80 00 00 00    	cmp    $0x80,%ebx
	81 fb 97 00 00 00    	cmp    $0x97,%ebx
	81 fb df 07 00 00    	cmp    $0x7df,%ebx
	81 fb f4 01 00 00    	cmp    $0x1f4,%ebx
	81 fe 78 04 10 00    	cmp    $0x100478,%esi
	83 45 e0 08          	addl   $0x8,-0x20(%ebp)
	83 45 e4 28          	addl   $0x28,-0x1c(%ebp)
	83 45 e4 40          	addl   $0x40,-0x1c(%ebp)
	83 7d e4 02          	cmpl   $0x2,-0x1c(%ebp)
	83 7d e4 04          	cmpl   $0x4,-0x1c(%ebp)
	83 c0 02             	add    $0x2,%eax
	83 c0 03             	add    $0x3,%eax
	83 c0 04             	add    $0x4,%eax
	83 c0 09             	add    $0x9,%eax
	83 c2 04             	add    $0x4,%edx
	83 c3 02             	add    $0x2,%ebx
	83 c3 04             	add    $0x4,%ebx
	83 c3 20             	add    $0x20,%ebx
	83 c3 28             	add    $0x28,%ebx
	83 c4 0c             	add    $0xc,%esp
	83 c4 10             	add    $0x10,%esp
	83 c4 18             	add    $0x18,%esp
	83 c4 20             	add    $0x20,%esp
	83 c6 02             	add    $0x2,%esi
	83 c6 10             	add    $0x10,%esi
	83 c6 20             	add    $0x20,%esi
	83 c7 04             	add    $0x4,%edi
	83 e0 01             	and    $0x1,%eax
	83 e1 07             	and    $0x7,%ecx
	83 e4 f0             	and    $0xfffffff0,%esp
	83 e6 01             	and    $0x1,%esi
	83 ec 08             	sub    $0x8,%esp
	83 ec 0c             	sub    $0xc,%esp
	83 ec 10             	sub    $0x10,%esp
	83 ec 14             	sub    $0x14,%esp
	83 ec 18             	sub    $0x18,%esp
	83 ec 1c             	sub    $0x1c,%esp
	83 f8 01             	cmp    $0x1,%eax
	83 f8 04             	cmp    $0x4,%eax
	83 f8 08             	cmp    $0x8,%eax
	83 f8 0a             	cmp    $0xa,%eax
	83 f8 0b             	cmp    $0xb,%eax
	83 f8 0c             	cmp    $0xc,%eax
	83 f8 14             	cmp    $0x14,%eax
	83 f8 32             	cmp    $0x32,%eax
	83 f8 40             	cmp    $0x40,%eax
	83 f8 64             	cmp    $0x64,%eax
	83 f8 65             	cmp    $0x65,%eax
	83 f9 02             	cmp    $0x2,%ecx
	83 f9 0a             	cmp    $0xa,%ecx
	83 f9 0b             	cmp    $0xb,%ecx
	83 fa 01             	cmp    $0x1,%edx
	83 fb 01             	cmp    $0x1,%ebx
	83 fb 02             	cmp    $0x2,%ebx
	83 fb 08             	cmp    $0x8,%ebx
	83 fb 0a             	cmp    $0xa,%ebx
	83 fb 0c             	cmp    $0xc,%ebx
	83 fb 0d             	cmp    $0xd,%ebx
	83 fb 0e             	cmp    $0xe,%ebx
	83 fb 14             	cmp    $0x14,%ebx
	83 fb 1e             	cmp    $0x1e,%ebx
	83 fb 1f             	cmp    $0x1f,%ebx
	83 fb 20             	cmp    $0x20,%ebx
	83 fb 28             	cmp    $0x28,%ebx
	83 fb ff             	cmp    $0xffffffff,%ebx
	83 fe 01             	cmp    $0x1,%esi
	83 fe 02             	cmp    $0x2,%esi
	83 fe 03             	cmp    $0x3,%esi
	83 fe 04             	cmp    $0x4,%esi
	83 fe 0a             	cmp    $0xa,%esi
	83 fe 11             	cmp    $0x11,%esi
	83 fe 13             	cmp    $0x13,%esi
	83 ff 02             	cmp    $0x2,%edi
	83 ff 04             	cmp    $0x4,%edi
	83 ff 08             	cmp    $0x8,%edi
	84 04 11             	test   %al,(%ecx,%edx,1)
	84 c0                	test   %al,%al
	85 c0                	test   %eax,%eax
	85 c9                	test   %ecx,%ecx
	85 d2                	test   %edx,%edx
	88 02                	mov    %al,(%edx)
	88 d0                	mov    %dl,%al
	88 d8                	mov    %bl,%al
	88 d9                	mov    %bl,%cl
	89 04 24             	mov    %eax,(%esp)
	89 04 85 00 01 10 00 	mov    %eax,0x100100(,%eax,4)
	89 04 9d 00 01 10 00 	mov    %eax,0x100100(,%ebx,4)
	89 04 9d 80 01 10 00 	mov    %eax,0x100180(,%ebx,4)
	89 0c 85 9c 01 10 00 	mov    %ecx,0x10019c(,%eax,4)
	89 0c b5 bc 01 10 00 	mov    %ecx,0x1001bc(,%esi,4)
	89 0f                	mov    %ecx,(%edi)
	89 10                	mov    %edx,(%eax)
	89 14 85 a0 01 10 00 	mov    %edx,0x1001a0(,%eax,4)
	89 14 8d 00 01 10 00 	mov    %edx,0x100100(,%ecx,4)
	89 14 9d 40 01 10 00 	mov    %edx,0x100140(,%ebx,4)
	89 16                	mov    %edx,(%esi)
	89 17                	mov    %edx,(%edi)
	89 3c 9d c0 01 10 00 	mov    %edi,0x1001c0(,%ebx,4)
	89 45 08             	mov    %eax,0x8(%ebp)
	89 45 0c             	mov    %eax,0xc(%ebp)
	89 45 e0             	mov    %eax,-0x20(%ebp)
	89 45 e4             	mov    %eax,-0x1c(%ebp)
	89 45 ec             	mov    %eax,-0x14(%ebp)
	89 45 f0             	mov    %eax,-0x10(%ebp)
	89 4d e4             	mov    %ecx,-0x1c(%ebp)
	89 55 08             	mov    %edx,0x8(%ebp)
	89 55 e0             	mov    %edx,-0x20(%ebp)
	89 55 f4             	mov    %edx,-0xc(%ebp)
	89 55 fc             	mov    %edx,-0x4(%ebp)
	89 5d ec             	mov    %ebx,-0x14(%ebp)
	89 75 d8             	mov    %esi,-0x28(%ebp)
	89 75 e8             	mov    %esi,-0x18(%ebp)
	89 75 ec             	mov    %esi,-0x14(%ebp)
	89 7d dc             	mov    %edi,-0x24(%ebp)
	89 7d e4             	mov    %edi,-0x1c(%ebp)
	89 7d ec             	mov    %edi,-0x14(%ebp)
	89 7d f0             	mov    %edi,-0x10(%ebp)
	89 8f 00 06 10 00    	mov    %ecx,0x100600(%edi)
	89 c1                	mov    %eax,%ecx
	89 c2                	mov    %eax,%edx
	89 c3                	mov    %eax,%ebx
	89 c6                	mov    %eax,%esi
	89 c7                	mov    %eax,%edi
	89 c8                	mov    %ecx,%eax
	89 ca                	mov    %ecx,%edx
	89 d0                	mov    %edx,%eax
	89 d1                	mov    %edx,%ecx
	89 d6                	mov    %edx,%esi
	89 d7                	mov    %edx,%edi
	89 d8                	mov    %ebx,%eax
	89 da                	mov    %ebx,%edx
	89 de                	mov    %ebx,%esi
	89 e5                	mov    %esp,%ebp
	89 f0                	mov    %esi,%eax
	89 f1                	mov    %esi,%ecx
	89 f3                	mov    %esi,%ebx
	89 f7                	mov    %esi,%edi
	89 f8                	mov    %edi,%eax
	89 f9                	mov    %edi,%ecx
	89 fe                	mov    %edi,%esi
	8b 02                	mov    (%edx),%eax
	8b 04 85 e0 00 10 00 	mov    0x1000e0(,%eax,4),%eax
	8b 04 9d 00 01 10 00 	mov    0x100100(,%ebx,4),%eax
	8b 04 9d 20 01 10 00 	mov    0x100120(,%ebx,4),%eax
	8b 04 9d 70 01 10 00 	mov    0x100170(,%ebx,4),%eax
	8b 04 9d 78 01 10 00 	mov    0x100178(,%ebx,4),%eax
	8b 04 9d 80 01 10 00 	mov    0x100180(,%ebx,4),%eax
	8b 04 9d e0 00 10 00 	mov    0x1000e0(,%ebx,4),%eax
	8b 04 b5 b0 01 10 00 	mov    0x1001b0(,%esi,4),%eax
	8b 04 bd 60 02 10 00 	mov    0x100260(,%edi,4),%eax
	8b 04 dd 58 01 10 00 	mov    0x100158(,%ebx,8),%eax
	8b 0c 85 a0 01 10 00 	mov    0x1001a0(,%eax,4),%ecx
	8b 0c bd b0 01 10 00 	mov    0x1001b0(,%edi,4),%ecx
	8b 0d a4 01 10 00    	mov    0x1001a4,%ecx
	8b 13                	mov    (%ebx),%edx
	8b 14 85 60 02 10 00 	mov    0x100260(,%eax,4),%edx
	8b 14 85 a0 01 10 00 	mov    0x1001a0(,%eax,4),%edx
	8b 14 85 c0 01 10 00 	mov    0x1001c0(,%eax,4),%edx
	8b 14 8d 00 01 10 00 	mov    0x100100(,%ecx,4),%edx
	8b 14 bd 00 02 10 00 	mov    0x100200(,%edi,4),%edx
	8b 14 bd 20 02 10 00 	mov    0x100220(,%edi,4),%edx
	8b 14 dd 5c 01 10 00 	mov    0x10015c(,%ebx,8),%edx
	8b 16                	mov    (%esi),%edx
	8b 17                	mov    (%edi),%edx
	8b 1c 85 20 02 10 00 	mov    0x100220(,%eax,4),%ebx
	8b 1c 85 60 02 10 00 	mov    0x100260(,%eax,4),%ebx
	8b 1d 44 02 10 00    	mov    0x100244,%ebx
	8b 1d 48 02 10 00    	mov    0x100248,%ebx
	8b 34 85 00 02 10 00 	mov    0x100200(,%eax,4),%esi
	8b 35 40 02 10 00    	mov    0x100240,%esi
	8b 3c b5 bc 01 10 00 	mov    0x1001bc(,%esi,4),%edi
	8b 3f                	mov    (%edi),%edi
	8b 45 08             	mov    0x8(%ebp),%eax
	8b 45 0c             	mov    0xc(%ebp),%eax
	8b 45 10             	mov    0x10(%ebp),%eax
	8b 45 d8             	mov    -0x28(%ebp),%eax
	8b 45 dc             	mov    -0x24(%ebp),%eax
	8b 45 e0             	mov    -0x20(%ebp),%eax
	8b 45 e4             	mov    -0x1c(%ebp),%eax
	8b 45 f0             	mov    -0x10(%ebp),%eax
	8b 45 f4             	mov    -0xc(%ebp),%eax
	8b 4d 08             	mov    0x8(%ebp),%ecx
	8b 4d 0c             	mov    0xc(%ebp),%ecx
	8b 4d 14             	mov    0x14(%ebp),%ecx
	8b 4d f0             	mov    -0x10(%ebp),%ecx
	8b 4d fc             	mov    -0x4(%ebp),%ecx
	8b 55 08             	mov    0x8(%ebp),%edx
	8b 55 0c             	mov    0xc(%ebp),%edx
	8b 55 10             	mov    0x10(%ebp),%edx
	8b 55 14             	mov    0x14(%ebp),%edx
	8b 55 dc             	mov    -0x24(%ebp),%edx
	8b 55 e4             	mov    -0x1c(%ebp),%edx
	8b 55 f4             	mov    -0xc(%ebp),%edx
	8b 55 fc             	mov    -0x4(%ebp),%edx
	8b 5d 08             	mov    0x8(%ebp),%ebx
	8b 5d 0c             	mov    0xc(%ebp),%ebx
	8b 5d e0             	mov    -0x20(%ebp),%ebx
	8b 5d f0             	mov    -0x10(%ebp),%ebx
	8b 75 10             	mov    0x10(%ebp),%esi
	8b 75 ec             	mov    -0x14(%ebp),%esi
	8b 7d 08             	mov    0x8(%ebp),%edi
	8b 7d e4             	mov    -0x1c(%ebp),%edi
	8b 7d e8             	mov    -0x18(%ebp),%edi
	8b 7d ec             	mov    -0x14(%ebp),%edi
	8b 84 df 24 01 10 00 	mov    0x100124(%edi,%ebx,8),%eax
	8b 8c df 20 01 10 00 	mov    0x100120(%edi,%ebx,8),%ecx
	8b b0 20 03 10 00    	mov    0x100320(%eax),%esi
	8b b8 24 03 10 00    	mov    0x100324(%eax),%edi
	8d 04 88             	lea    (%eax,%ecx,4),%eax
	8d 14 3f             	lea    (%edi,%edi,1),%edx
	8d 1c 8d 00 00 00 00 	lea    0x0(,%ecx,4),%ebx
	8d 34 87             	lea    (%edi,%eax,4),%esi
	8d 3c 87             	lea    (%edi,%eax,4),%edi
	8d 3c 8f             	lea    (%edi,%ecx,4),%edi
	8d 41 01             	lea    0x1(%ecx),%eax
	8d 42 20             	lea    0x20(%edx),%eax
	8d 43 20             	lea    0x20(%ebx),%eax
	8d 43 bf             	lea    -0x41(%ebx),%eax
	8d 4a bf             	lea    -0x41(%edx),%ecx
	8d 4c 24 04          	lea    0x4(%esp),%ecx
	8d 50 ff             	lea    -0x1(%eax),%edx
	8d 58 01             	lea    0x1(%eax),%ebx
	8d 5c 1f 04          	lea    0x4(%edi,%ebx,1),%ebx
	8d 5d f6             	lea    -0xa(%ebp),%ebx
	8d 5e 01             	lea    0x1(%esi),%ebx
	8d 61 fc             	lea    -0x4(%ecx),%esp
	8d 65 f0             	lea    -0x10(%ebp),%esp
	8d 65 f4             	lea    -0xc(%ebp),%esp
	8d 65 f8             	lea    -0x8(%ebp),%esp
	8d 73 01             	lea    0x1(%ebx),%esi
	8d 73 04             	lea    0x4(%ebx),%esi
	8d 76 00             	lea    0x0(%esi),%esi
	8d 77 fc             	lea    -0x4(%edi),%esi
	8d 9e 70 fe ff ff    	lea    -0x190(%esi),%ebx
	90                   	nop
	99                   	cltd   
	a1 18 02 10 00       	mov    0x100218,%eax
	a1 1c 02 10 00       	mov    0x10021c,%eax
	a1 34 02 10 00       	mov    0x100234,%eax
	a1 38 02 10 00       	mov    0x100238,%eax
	a1 80 01 10 00       	mov    0x100180,%eax
	a1 84 01 10 00       	mov    0x100184,%eax
	a1 88 01 10 00       	mov    0x100188,%eax
	a1 c3 00 10 00       	mov    0x1000c3,%eax
	a3 18 02 10 00       	mov    %eax,0x100218
	a3 bc 00 10 00       	mov    %eax,0x1000bc
<add>:
	b8 01 00 00 00       	mov    $0x1,%eax
	b8 1f 85 eb 51       	mov    $0x51eb851f,%eax
	b8 32 00 00 00       	mov    $0x32,%eax
	b8 4b 00 00 00       	mov    $0x4b,%eax
	b8 56 55 55 55       	mov    $0x55555556,%eax
	b8 64 00 00 00       	mov    $0x64,%eax
	b8 96 00 00 00       	mov    $0x96,%eax
	b8 cd cc cc cc       	mov    $0xcccccccd,%eax
	b8 ff ff ff ff       	mov    $0xffffffff,%eax
	b9 01 00 00 00       	mov    $0x1,%ecx
	b9 02 00 00 00       	mov    $0x2,%ecx
	b9 0a 00 00 00       	mov    $0xa,%ecx
	b9 64 00 00 00       	mov    $0x64,%ecx
	b9 90 01 00 00       	mov    $0x190,%ecx
	ba 01 00 00 00       	mov    $0x1,%edx
	bb 01 00 00 00       	mov    $0x1,%ebx
	bb 02 00 00 00       	mov    $0x2,%ebx
	bb 04 00 00 00       	mov    $0x4,%ebx
	bb 13 00 00 00       	mov    $0x13,%ebx
	bb 62 07 00 00       	mov    $0x762,%ebx
	bb 64 00 00 00       	mov    $0x64,%ebx
	bb 65 00 00 00       	mov    $0x65,%ebx
	bc 00 7c 00 00       	mov    $0x7c00,%esp
	bd 00 00 00 00       	mov    $0x0,%ebp
	be 01 00 00 00       	mov    $0x1,%esi
	be 50 04 10 00       	mov    $0x100450,%esi
	be 90 01 00 00       	mov    $0x190,%esi
	bf 64 00 00 00       	mov    $0x64,%edi
<bubble_sort>:
	c1 e0 06             	shl    $0x6,%eax
	c1 e8 03             	shr    $0x3,%eax
	c1 e8 07             	shr    $0x7,%eax
	c1 ee 05             	shr    $0x5,%esi
	c1 f9 1f             	sar    $0x1f,%ecx
	c1 fa 03             	sar    $0x3,%edx
	c3                   	ret    
	c6 05 c8 01 10 00 61 	movb   $0x61,0x1001c8
	c6 05 c9 01 10 00 61 	movb   $0x61,0x1001c9
	c6 05 c9 01 10 00 80 	movb   $0x80,0x1001c9
	c7 04 24 01 00 00 00 	movl   $0x1,(%esp)
	c7 04 24 02 00 00 00 	movl   $0x2,(%esp)
	c7 04 24 0c 00 00 00 	movl   $0xc,(%esp)
	c7 04 9d a0 01 10 00 	movl   $0x1,0x1001a0(,%ebx,4)
	c7 05 20 01 10 00 03 	movl   $0x3,0x100120
	c7 05 40 01 10 00 00 	movl   $0x0,0x100140
	c7 05 44 01 10 00 01 	movl   $0x1,0x100144
	c7 05 48 01 10 00 02 	movl   $0x2,0x100148
	c7 05 4c 01 10 00 03 	movl   $0x3,0x10014c
	c7 05 50 01 10 00 04 	movl   $0x4,0x100150
	c7 05 54 01 10 00 03 	movl   $0x3,0x100154
	c7 05 80 01 10 00 03 	movl   $0x3,0x100180
	c7 05 a0 01 10 00 00 	movl   $0x0,0x1001a0
	c7 05 a0 01 10 00 01 	movl   $0x1,0x1001a0
	c7 05 a0 01 10 00 61 	movl   $0x61,0x1001a0
	c7 05 a0 01 10 00 80 	movl   $0xffffff80,0x1001a0
	c7 05 a4 01 10 00 01 	movl   $0x1,0x1001a4
	c7 05 a8 01 10 00 02 	movl   $0x2,0x1001a8
	c7 05 ac 01 10 00 03 	movl   $0x3,0x1001ac
	c7 05 b0 01 10 00 04 	movl   $0x4,0x1001b0
	c7 05 b4 01 10 00 03 	movl   $0x3,0x1001b4
	c7 05 c3 00 10 00 dd 	movl   $0xaabbccdd,0x1000c3
	c7 45 dc 00 00 00 00 	movl   $0x0,-0x24(%ebp)
	c7 45 e0 00 00 00 00 	movl   $0x0,-0x20(%ebp)
	c7 45 e4 00 00 00 00 	movl   $0x0,-0x1c(%ebp)
	c7 45 e4 60 04 10 00 	movl   $0x100460,-0x1c(%ebp)
	c7 45 f0 00 00 00 00 	movl   $0x0,-0x10(%ebp)
	c7 45 f0 01 00 00 00 	movl   $0x1,-0x10(%ebp)
	c7 45 f4 00 00 00 00 	movl   $0x0,-0xc(%ebp)
	c9                   	leave  
	cc bb aa 
<cube>:
	d1 fe                	sar    %esi
	d3 e0                	shl    %cl,%eax
	d3 e8                	shr    %cl,%eax
	d3 f8                	sar    %cl,%eax
	d6                   	(bad)  
	e8 01 00 00 00       	call   100010 <_trm_init>
	e8 03 ff ff ff       	call   10002c <nemu_assert>
	e8 05 00 00 00       	call   100020 <main>
	e8 09 ff ff ff       	call   10002c <nemu_assert>
	e8 0d 00 00 00       	call   10001c <_trm_init>
	e8 0d 01 00 00       	call   10019c <strcmp>
	e8 0e ff ff ff       	call   10002c <nemu_assert>
	e8 15 ff ff ff       	call   10002c <nemu_assert>
	e8 15 ff ff ff       	call   100048 <getbit>
	e8 16 ff ff ff       	call   10002c <nemu_assert>
	e8 1a ff ff ff       	call   10002c <nemu_assert>
	e8 1a ff ff ff       	call   100048 <partition>
	e8 1d ff ff ff       	call   10002c <nemu_assert>
	e8 20 ff ff ff       	call   10002c <nemu_assert>
	e8 21 00 00 00       	call   100048 <main>
	e8 21 ff ff ff       	call   10002c <nemu_assert>
	e8 22 00 00 00       	call   100118 <strcmp>
	e8 22 fe ff ff       	call   10002c <nemu_assert>
	e8 22 ff ff ff       	call   10002c <nemu_assert>
	e8 26 ff ff ff       	call   10002c <nemu_assert>
	e8 2a ff ff ff       	call   10002c <nemu_assert>
	e8 2c ff ff ff       	call   10002c <nemu_assert>
	e8 2d 00 00 00       	call   100054 <main>
	e8 2d 01 00 00       	call   100154 <main>
	e8 2d ff ff ff       	call   10002c <nemu_assert>
	e8 2f ff ff ff       	call   100048 <getbit>
	e8 30 00 00 00       	call   100114 <sprintf>
	e8 30 00 00 00       	call   1001a4 <memcmp>
	e8 30 01 00 00       	call   10019c <strcmp>
	e8 31 00 00 00       	call   100058 <main>
	e8 32 ff ff ff       	call   10002c <nemu_assert>
	e8 33 ff ff ff       	call   10002c <nemu_assert>
	e8 35 00 00 00       	call   10005c <main>
	e8 37 ff ff ff       	call   10002c <nemu_assert>
	e8 38 00 00 00       	call   1001a0 <memset>
	e8 39 00 00 00       	call   100060 <main>
	e8 39 ff ff ff       	call   10002c <nemu_assert>
	e8 3b ff ff ff       	call   10002c <nemu_assert>
	e8 3d 00 00 00       	call   100064 <main>
	e8 3e ff ff ff       	call   10002c <nemu_assert>
	e8 3f ff ff ff       	call   10002c <nemu_assert>
	e8 41 00 00 00       	call   100068 <main>
	e8 42 ff ff ff       	call   10002c <nemu_assert>
	e8 44 ff ff ff       	call   10002c <nemu_assert>
	e8 45 01 00 00       	call   10016c <main>
	e8 46 ff ff ff       	call   10002c <nemu_assert>
	e8 49 ff ff ff       	call   100048 <getbit>
	e8 4a ff ff ff       	call   10002c <nemu_assert>
	e8 4c fe ff ff       	call   10002c <nemu_assert>
	e8 4c ff ff ff       	call   10002c <nemu_assert>
	e8 4d ff ff ff       	call   10002c <nemu_assert>
	e8 4e ff ff ff       	call   10008c <goldbach>
	e8 4f ff ff ff       	call   10002c <nemu_assert>
	e8 50 ff ff ff       	call   10002c <nemu_assert>
	e8 51 ff ff ff       	call   10002c <nemu_assert>
	e8 55 00 00 00       	call   10007c <main>
	e8 56 ff ff ff       	call   10002c <nemu_assert>
	e8 57 ff ff ff       	call   10002c <nemu_assert>
	e8 5a ff ff ff       	call   10002c <nemu_assert>
	e8 5b 00 00 00       	call   10019c <strcmp>
	e8 5e 00 00 00       	call   100118 <strcmp>
	e8 5e ff ff ff       	call   10002c <nemu_assert>
	e8 5f fe ff ff       	call   10002c <nemu_assert>
	e8 5f ff ff ff       	call   10002c <nemu_assert>
	e8 5f ff ff ff       	call   100110 <quick_sort>
	e8 60 00 00 00       	call   100198 <strcat>
	e8 60 ff ff ff       	call   10002c <nemu_assert>
	e8 61 ff ff ff       	call   10002c <nemu_assert>
	e8 62 ff ff ff       	call   10002c <nemu_assert>
	e8 63 ff ff ff       	call   10002c <nemu_assert>
	e8 63 ff ff ff       	call   100048 <getbit>
	e8 64 ff ff ff       	call   10002c <nemu_assert>
	e8 65 00 00 00       	call   10008c <main>
	e8 65 00 00 00       	call   100194 <strcpy>
	e8 69 00 00 00       	call   100090 <main>
	e8 6a ff ff ff       	call   10002c <nemu_assert>
	e8 6c 00 00 00       	call   100114 <sprintf>
	e8 6c ff ff ff       	call   10002c <nemu_assert>
	e8 6e ff ff ff       	call   10002c <nemu_assert>
	e8 6f fe ff ff       	call   100068 <setbit>
	e8 72 ff ff ff       	call   10002c <nemu_assert>
	e8 75 ff ff ff       	call   10002c <nemu_assert>
	e8 7a ff ff ff       	call   10002c <nemu_assert>
	e8 7c fe ff ff       	call   100068 <setbit>
	e8 7c ff ff ff       	call   10002c <nemu_assert>
	e8 7d ff ff ff       	call   10002c <nemu_assert>
	e8 7d ff ff ff       	call   100048 <getbit>
	e8 7f ff ff ff       	call   10002c <nemu_assert>
	e8 80 ff ff ff       	call   100150 <nemu_assert>
	e8 81 00 00 00       	call   1000a8 <main>
	e8 81 ff ff ff       	call   10002c <nemu_assert>
	e8 82 ff ff ff       	call   10002c <nemu_assert>
	e8 85 ff ff ff       	call   10002c <nemu_assert>
	e8 86 ff ff ff       	call   10002c <nemu_assert>
	e8 89 fe ff ff       	call   100068 <setbit>
	e8 8c fe ff ff       	call   10002c <nemu_assert>
	e8 8d ff ff ff       	call   10002c <nemu_assert>
	e8 8e ff ff ff       	call   10002c <nemu_assert>
	e8 92 ff ff ff       	call   10002c <nemu_assert>
	e8 94 ff ff ff       	call   10002c <nemu_assert>
	e8 96 fe ff ff       	call   100068 <setbit>
	e8 99 00 00 00       	call   10019c <strcmp>
	e8 99 ff ff ff       	call   100150 <nemu_assert>
	e8 9a 00 00 00       	call   100118 <strcmp>
	e8 9a ff ff ff       	call   10002c <nemu_assert>
	e8 9b fe ff ff       	call   10002c <nemu_assert>
	e8 9d ff ff ff       	call   10002c <nemu_assert>
	e8 9e ff ff ff       	call   10002c <nemu_assert>
	e8 9e ff ff ff       	call   100110 <quick_sort>
	e8 9f fe ff ff       	call   10002c <nemu_assert>
	e8 9f ff ff ff       	call   10002c <nemu_assert>
	e8 a3 fe ff ff       	call   100068 <setbit>
	e8 a3 ff ff ff       	call   10002c <nemu_assert>
	e8 a5 ff ff ff       	call   10002c <nemu_assert>
	e8 a7 00 00 00       	call   100114 <sprintf>
	e8 a7 fe ff ff       	call   100010 <_halt>
	e8 a8 fe ff ff       	call   10002c <nemu_assert>
	e8 a8 ff ff ff       	call   10002c <nemu_assert>
	e8 ae ff ff ff       	call   10002c <nemu_assert>
	e8 b0 fe ff ff       	call   100068 <setbit>
	e8 b0 ff ff ff       	call   10002c <nemu_assert>
	e8 b2 ff ff ff       	call   100150 <nemu_assert>
	e8 b4 fe ff ff       	call   10002c <nemu_assert>
	e8 b5 fe ff ff       	call   10002c <nemu_assert>
	e8 bd fe ff ff       	call   100068 <setbit>
	e8 c1 00 00 00       	call   10019c <strcmp>
	e8 c4 fe ff ff       	call   10002c <nemu_assert>
	e8 c7 fe ff ff       	call   10002c <nemu_assert>
	e8 c7 fe ff ff       	call   100048 <getbit>
	e8 ca fe ff ff       	call   100068 <setbit>
	e8 cb ff ff ff       	call   100010 <_halt>
	e8 cf fe ff ff       	call   10002c <nemu_assert>
	e8 d1 ff ff ff       	call   100110 <quick_sort>
	e8 d7 fe ff ff       	call   10002c <nemu_assert>
	e8 db fe ff ff       	call   10002c <nemu_assert>
	e8 de fe ff ff       	call   10002c <nemu_assert>
	e8 e1 fe ff ff       	call   100048 <getbit>
	e8 e5 fe ff ff       	call   10002c <nemu_assert>
	e8 e9 00 00 00       	call   10019c <strcmp>
	e8 e9 fe ff ff       	call   10002c <nemu_assert>
	e8 f1 fe ff ff       	call   10002c <nemu_assert>
	e8 f8 fe ff ff       	call   10002c <nemu_assert>
	e8 fb fe ff ff       	call   100048 <getbit>
	e8 fd 00 00 00       	call   100124 <main>
	e8 fd fe ff ff       	call   10002c <nemu_assert>
	e9 77 ff ff ff       	jmp    10006d <partition+0x25>
	eb 05                	jmp    10007e <main+0x36>
	eb 09                	jmp    100071 <is_prime+0x29>
	eb 09                	jmp    10008d <main+0x45>
	eb 09                	jmp    1000e0 <partition+0x98>
	eb 0c                	jmp    1000bd <goldbach+0x31>
	eb 0c                	jmp    1000ed <goldbach+0x61>
	eb 33                	jmp    1000d4 <main+0x58>
	eb 34                	jmp    1000d3 <main+0x47>
	eb a0                	jmp    10009d <partition+0x55>
	eb ae                	jmp    10007c <main+0x18>
	eb af                	jmp    100096 <main+0x4e>
	eb b1                	jmp    10009d <partition+0x55>
	eb b5                	jmp    1000b4 <main+0x28>
	eb bd                	jmp    1000b4 <main+0x28>
	eb c1                	jmp    10007c <main+0x18>
	eb c1                	jmp    10009c <main+0x34>
	eb c4                	jmp    100078 <main+0x30>
	eb c5                	jmp    1000b4 <main+0x28>
	eb c8                	jmp    10007c <main+0x18>
	eb cb                	jmp    1000b2 <main+0x36>
	eb cc                	jmp    1000b4 <main+0x28>
	eb cf                	jmp    100074 <main+0x14>
	eb d3                	jmp    10007c <main+0x18>
	eb d4                	jmp    100074 <main+0x14>
	eb e9                	jmp    1000fa <main+0x92>
	eb eb                	jmp    100076 <is_prime+0x2e>
	eb fe                	jmp    100017 <_halt+0x7>
	eb fe                	jmp    10001c <_trm_init+0xc>
	eb fe                	jmp    100028 <_trm_init+0xc>
<f3>:
	f6 45 08 03          	testb  $0x3,0x8(%ebp)
	f6 c1 01             	test   $0x1,%cl
	f6 c3 03             	test   $0x3,%bl
	f7 6d e4             	imull  -0x1c(%ebp)
	f7 d0                	not    %eax
	f7 e2                	mul    %edx
	f7 e3                	mul    %ebx
	f7 e9                	imul   %ecx
	f7 f9                	idiv   %ecx
	f7 fb                	idiv   %ebx
	f7 fe                	idiv   %esi
	f7 ff                	idiv   %edi
<fact>:
	fe c8                	dec    %al
	ff 05 1c 02 10 00    	incl   0x10021c
	ff 15 08 02 10 00    	call   *0x100208
	ff 15 0c 02 10 00    	call   *0x10020c
	ff 15 10 02 10 00    	call   *0x100210
	ff 25 08 02 10 00    	jmp    *0x100208
	ff 25 14 02 10 00    	jmp    *0x100214
	ff 35 34 02 10 00    	pushl  0x100234
	ff 35 38 02 10 00    	pushl  0x100238
	ff 35 3c 02 10 00    	pushl  0x10023c
	ff 45 dc             	incl   -0x24(%ebp)
	ff 45 e4             	incl   -0x1c(%ebp)
	ff 71 fc             	pushl  -0x4(%ecx)
	ff ff ff 
