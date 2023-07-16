#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
#define MAX_DETECT_PAGES 64
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  struct proc *p = myproc();
  uint64 mask = 0;
  uint64 uvaddr_buf;
  uint64 uvaddr_abits;
  pte_t *pte;
  int n;
  int ret = 0;

  argint(1, &n);
  if(n > MAX_DETECT_PAGES){
    printf("pgaccess: too large number of pages\n");
    return -1;
  }

  argaddr(0, &uvaddr_buf);
  argaddr(2, &uvaddr_abits);
  for(int i = 0; i < n; ++i){
    if((pte = walk(p->pagetable, uvaddr_buf + i * (1 << PGSHIFT), 0)) > 0){
      mask |= (((*pte & PTE_A) >> 6) << i);
      *pte = (*pte) & (~PTE_A);
    }
  }
  ret = copyout(p->pagetable, uvaddr_abits, (char *)&mask, sizeof(mask));
  return ret;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
