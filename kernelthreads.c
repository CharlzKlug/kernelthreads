#include <linux/module.h> /* For MODULE_AUTHOR,
			   * MODULE_LICENSE,
			   * MODULE_VERSION */
#include <linux/delay.h> /* For ssleep */
#include <linux/kthread.h> /* For kthread_create */
#include <linux/signal.h> /* For allow_signal */
#include <linux/sched/signal.h> /* For signal_pending */

MODULE_AUTHOR("<insert your name here>");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

static struct task_struct *thread_st;

/* // Function executed by kernel thread */
static int thread_fn(void *unused) {

  // Allow the SIGKILL signal
  allow_signal(SIGKILL);
  while (!kthread_should_stop()) {
    printk(KERN_INFO "Thread Running\n");
    ssleep(5);

    if (signal_pending(thread_st)) {
      break;
    }
  }

  printk(KERN_INFO "Thread Stopping\n");
  do_exit(0);
  return 0;
}

// Module initialization
static int __init init_thread(void) {
  printk(KERN_INFO "Creating Thread\n");

  // Create the kernel thread with name 'mythread'
  /* thread_st= kthread_create(thread_fn, NULL, "mythread"); */
  thread_st= kthread_run(thread_fn, NULL, "mythread");
  if (thread_st) {
    printk("Thread Created successfully\n");
    /* wake_up_process(thread_st); */
  } else {
    printk(KERN_INFO "Thread creation failed\n");
  }

  return 0;
}

// Module Exit
static void __exit cleanup_thread(void) {
  printk("Cleaning Up\n");

  if (thread_st) {
    kthread_stop(thread_st);
    printk(KERN_INFO "Thread stopped\n");
  }
}

module_init(init_thread);
module_exit(cleanup_thread);
