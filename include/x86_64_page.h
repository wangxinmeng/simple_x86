#ifndef __X86_64_PAGE_H__
#define __X86_64_PAGE_H__

/*  页表属性
    base bit12 ~31, 页表项对应的物理基地址
    p    bit0, present 物理页存在标志位， 0不存在，1存在
    rw   bit1, read/write 读写标志位, 0只读，1可读写
    us   bit2, user/supervisor 访问模式，用户/超级，0超级，1用户
    pwt  bit3, page-level write-through 页写穿标志位，0(write-back)，1(write-through)
    pcd  bit4, page-level cache disable 页禁止缓存标志位，0可以缓存，1禁止缓存
    a    bit5, accessed 访问标志位， 0未访问，1已访问
    d    bit6, dirty 脏页标志位，0干净，1已脏
    pat  bit7, page-attribute table 页面属性标志位
         bit12, 在long模式下的PDPTE和PDE表项，位于bit12
    ps   bit7, 在long模式下的PDPTE和PDE表项中，位于bit7
    g    bit8, global 全局标志位，0局部页面，1全局页面
            当CR3刷新时，局部页面会被刷新，而全局页面不会
*/
#define P_PAGE_PRE              1
#define P_PAGE_IGNORE           0

#define P_PAGE_READ_ONLY        0
#define P_PAGE_READ_WRITE       1

#define P_PAGE_SUPER            0 // 只能0/1/2权限才能访问
#define P_PAGE_USER             1 // 0/1/2/3权限均可访问

#define P_PAGE_WRITE_BACK       0
#define P_PAGE_WRITE_THROUGH    1

#define P_PAGE_CACHE_ENABLE     0
#define P_PAGE_CACHE_DISABLE    1

#define P_PAGE_ACCESSED_NO      0
#define P_PAGE_ACCESSED         1

#define P_PAGE_DIRTYED          1
#define P_PAGE_NOT_DIRTYED      0

#define P_PAGE_PAT0             0
#define P_PAGE_PAT1             1

#define P_PAGE_GLOBAL           1
#define P_PAGE_LOCAL            0

/* 32位模式下的页表结构，页表项占4B */
#define P_PAGE_TABLE_ENTRY(base, p, rw, us, pwt, pcd, a, d, pat, g) \
    ((B_SEL(p,0,1,0))|(B_SEL(rw,0,1,1))|(B_SEL(us,0,1,2))|(B_SEL(pwt,0,1,3)) \
    |(B_SEL(pcd,0,1,4))|(B_SEL(a,0,1,5))|(B_SEL(d,0,1,6))|(B_SEL(pat,0,1,7)) \
    | ((B_SEL(g,0,1,8))|(B_SEL(base,0,20,12))))

/* CR3页表项 */
#define P_CR3(base, pwt, pcd) \
    P_PAGE_TABLE_ENTRY(base, P_PAGE_IGNORE, P_PAGE_IGNORE, P_PAGE_IGNORE, \
        pwt, pcd, P_PAGE_IGNORE, P_PAGE_IGNORE, P_PAGE_IGNORE, P_PAGE_IGNORE)
/* PDE页表项 */
#define P_PDE(base, rw, us, pwt, pcd, a) \
    P_PAGE_TABLE_ENTRY(base, P_PAGE_PRE, rw, us, pwt, pcd, a, P_PAGE_IGNORE, P_PAGE_PAT0, P_PAGE_IGNORE)
/* PTE页表项 */
#define P_PTE(base, rw, us, pwt, pcd, a, d, pat, g) \
    P_PAGE_TABLE_ENTRY(base, P_PAGE_PRE, rw, us, pwt, pcd, a, d, pat, g)


/* 64位模式下的页表结构，页表项扩大到8B */
#define E_PAGE_TABLE_ENTRY(base, max_phy, p, rw, us, pwt, pcd, a, d, pat_ps, g, xd) \
    (P_PAGE_TABLE_ENTRY(base, p, rw, us, pwt, pcd, a, d, pat_ps, g) \
    | ((B_SEL(base,20,((max_phy)-20),32))| (B_SEL(xd,0,1,63))))

/* CR3页表项，根据pcid值，有不同功能 */
#define E_CR3(base, max_phy, pwt, pcd) \
    (P_CR3(base, pwt, pcd) \
    | (B_SEL(base,20,((max_phy)-20),32)))
#define E_CR3_PCID(base, max_phy, pcid) \
    (((B_SEL(pcid,0,12,0))|(B_SEL(base,0,20,12)))| (B_SEL(base,20,((max_phy)-20),32)))

/* XD Execution Disable 禁止执行功能 */
#define E_PAGE_XD_DISABLE  0
#define E_PAGE_XD_ENABLE   1

/* ps用来指示PDPTE/PDE当前项是否直接指向物理页，即2M/1G大页 */
#define E_PAGE_PS_TO_PHY   1
#define E_PAGE_PS_TO_TALBE 0

/* PML4页表项 */
#define E_PML4E(base, max_phy, rw, us, pwt, pcd, a, xd) \
    E_PAGE_TABLE_ENTRY(base, max_phy, P_PAGE_PRE, rw, us, pwt, pcd, a, \
        P_PAGE_IGNORE, P_PAGE_IGNORE, P_PAGE_IGNORE, xd)
/* PDPTE：页表 */
#define E_PDPTE(base, max_phy, rw, us, pwt, pcd, a, xd) \
    E_PAGE_TABLE_ENTRY(base, max_phy, P_PAGE_PRE, rw, us, pwt, pcd, a, P_PAGE_IGNORE, E_PAGE_PS_TO_TALBE, P_PAGE_IGNORE, xd)
/* PDPTE：1GB物理页 */
#define E_PDPTE_1GB(base, max_phy, rw, us, pwt, pcd, a, d, g, pat, xd) \
    E_PAGE_TABLE_ENTRY((B_SEL(base,0,((max_phy)-30),18))|(B_SEL(pat,0,1,0))), max_phy, \
        P_PAGE_PRE, rw, us, pwt, pcd, a, d, E_PAGE_PS_TO_PHY, g, xd)
/* PDE页表 */
#define E_PDE(base, max_phy, rw, us, pwt, pcd, a, xd) \
    E_PAGE_TABLE_ENTRY(base, max_phy, P_PAGE_PRE, rw, us, pwt, pcd, a, P_PAGE_IGNORE, E_PAGE_PS_TO_TALBE, P_PAGE_IGNORE, xd)
/* PDE：2MB物理页 */
#define E_PDE_2MB(base, max_phy, rw, us, pwt, pcd, a, d, g, pat, xd) \
    E_PAGE_TABLE_ENTRY(((B_SEL(base,0,((max_phy)-21),9))|(B_SEL(pat,0,1,0))), max_phy, \
        P_PAGE_PRE, rw, us, pwt, pcd, a, d, E_PAGE_PS_TO_PHY, g, xd)
/* PTE */
#define E_PTE(base, max_phy, rw, us, pwt, pcd, a, d, pat, g, xd) \
    (P_PTE(base, rw, us, pwt, pcd, a, d, pat, g) \
    | ((B_SEL(base,20,((max_phy)-20),32))| (B_SEL(xd,0,1,63))))

#endif
