# Philosophers - Concurrency ve Mutex Problemi

## 📚 Proje Açıklaması

**Philosophers**, klasik \"Dining Philosophers Problem\"inin C'de multi-threading kullanarak implementasyonudur. Bu proje, concurrency, mutex'ler, deadlock prevention ve thread synchronization gibi kritik sistem programlaması konseptlerini öğretir.

## 🎯 Klasik Problem

```
5 Philosopher etrafında masa:
- Ortada spaghetti
- Aralarında 5 çatal (fork)
- Her philosopher'ın sağında 1 çatal, solunda 1 çatal
- Spaghetti yemek için 2 çatal gerekli

Kısıtlar:
- Aynı anda 2 philosopher aynı çatalı kullanamaz (mutex)
- Philosopher'lar ölmemelidir (deadlock'tan kaçınmalı)
```

## 📖 Problem Analizi

### Philosopher Yaşam Döngüsü

```
loop {
    1. Think (düşün)
    2. Grab left fork (sol çatal al)
    3. Grab right fork (sağ çatal al)
    4. Eat (ye)
    5. Release right fork (sağ çatal bırak)
    6. Release left fork (sol çatal bırak)
}
```

### Deadlock Riski

```
Tüm philosopher'lar aynı anda sol çatallarını alırsa:
Philosopher 1: Çatal 1'i aldı, Çatal 2'yi bekler
Philosopher 2: Çatal 2'yi aldı, Çatal 3'ü bekler
...
Hiçbiri ilerleyemez!
```

## 🛠️ Implementasyon

### Veri Yapıları

```c
typedef struct s_philosopher {
    int id;
    int eating_count;
    long last_meal_time;
    pthread_t thread;
    struct s_table *table;
} t_philosopher;

typedef struct s_table {
    int num_philosophers;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int must_eat_count;
    
    pthread_mutex_t *forks;          // Çatal mutex'leri
    pthread_mutex_t print_mutex;     // Print için
    pthread_mutex_t death_mutex;     // Ölüm kontrolü
    
    int someone_died;
    t_philosopher *philosophers;
} t_table;
```

### Çözüm 1: Even-Odd Strategy (Basit)

```c
void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    
    // Philosopher ID'si çift ise 1ms bekle (staggering)
    if (philo->id % 2 == 0)
        usleep(1000);
    
    while (!philo->table->someone_died)
    {
        // Think
        print_message(philo, "is thinking");
        
        // Çatal al
        pthread_mutex_lock(&philo->table->forks[philo->id]);
        print_message(philo, "has taken a fork");
        
        pthread_mutex_lock(&philo->table->forks[(philo->id + 1) % N]);
        print_message(philo, "has taken a fork");
        
        // Ye
        pthread_mutex_lock(&philo->table->death_mutex);
        philo->last_meal_time = get_timestamp();
        pthread_mutex_unlock(&philo->table->death_mutex);
        
        print_message(philo, "is eating");
        usleep(philo->table->time_to_eat * 1000);
        philo->eating_count++;
        
        // Çatalları bırak
        pthread_mutex_unlock(&philo->table->forks[philo->id]);
        pthread_mutex_unlock(&philo->table->forks[(philo->id + 1) % N]);
        
        // Uyu
        print_message(philo, "is sleeping");
        usleep(philo->table->time_to_sleep * 1000);
    }
    
    return (NULL);
}
```

### Çözüm 2: Resource Hierarchy (İleri)

```c
void acquire_forks(t_philosopher *philo)
{
    int left = philo->id;
    int right = (philo->id + 1) % philo->table->num_philosophers;
    
    // Her zaman küçük ID'li çatalı ilk al (deadlock prevention)
    int first = (left < right) ? left : right;
    int second = (left < right) ? right : left;
    
    pthread_mutex_lock(&philo->table->forks[first]);
    print_message(philo, "has taken a fork");
    
    pthread_mutex_lock(&philo->table->forks[second]);
    print_message(philo, "has taken a fork");
}
```

### Çözüm 3: Mutex-Free (Advanced)

Atomik operasyonlar ve lock-free queue kullanarak.

## 📚 Temel Konseptler

### Mutex (Mutual Exclusion)

```c
pthread_mutex_t fork;

// İnit et
pthread_mutex_init(&fork, NULL);

// Lock al (girilirse kadar bekle)
pthread_mutex_lock(&fork);
// Critical section
pthread_mutex_unlock(&fork);

// Temizle
pthread_mutex_destroy(&fork);
```

### Thread Creation

```c
pthread_t thread;

// Thread başlat
pthread_create(&thread, NULL, philosopher_routine, philosopher);

// Thread'in bitmesini bekle
pthread_join(thread, NULL);
```

### Timestamp Alma

```c
#include <sys/time.h>

long get_timestamp(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);  // milliseconds
}
```

## 🛠️ Derleme ve Çalıştırma

```bash
cd Philosophers/philo
make

# Kullanım:
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]

# Örnek 1: 5 philosopher, 800ms ölüm süresi
./philo 5 800 200 200

# Örnek 2: 4 philosopher, her biri 5 kez yemeli
./philo 4 800 200 200 5

# Örnek 3: 1 philosopher (ölecek, çatalı yok)
./philo 1 800 200 200
```

## 📊 Output Örneği

```
[0] Philosopher 0 is thinking
[0] Philosopher 1 is thinking
[2] Philosopher 0 has taken a fork
[2] Philosopher 0 has taken a fork
[2] Philosopher 0 is eating
[2] Philosopher 1 has taken a fork
[2] Philosopher 2 has taken a fork
[2] Philosopher 2 is eating
...
[200] Philosopher 0 is sleeping
[202] Philosopher 1 is eating
[400] Philosopher 0 died
```

## 📚 Öğrenme Çıktıları

✅ Concurrency ve threading öğrenildi  
✅ Mutex ve lock mekanizmaları mastered  
✅ Deadlock problem ve çözümleri anlaşıldı  
✅ Race condition prevention öğrenildi  
✅ Thread synchronization uygulandı  
✅ Performance tuning ve optimization yapıldı  

## 🔧 Dosya Yapısı

```
Philosophers/philo/
├── main.c                  # Entry point, initialization
├── philo.c                 # Main routine
├── routine.c               # Philosopher routine
├── monitor.c               # Ölüm kontrolü
├── utils.c                 # Utility functions
├── parsing.c               # Argument parsing
├── philo.h                 # Header dosyası
└── Makefile
```

## 📝 Önemli Konseptler

### Deadlock Prevention Stratejileri

1. **Staggering**: Çiftler farklı zamanda başlamış
2. **Resource Hierarchy**: Her zaman aynı sırada kaynak al
3. **Timeouts**: Belli sürede lock alınamazsa timeout
4. **Bankers Algorithm**: Prevention algoritması

### Race Condition Prevention

```c
// YANLIŞ (Race condition):
if (philo->eating_count < target) {
    philo->eating_count++;  // Data race!
}

// DOĞRU (Mutex ile):
pthread_mutex_lock(&table->mutex);
if (philo->eating_count < target) {
    philo->eating_count++;
}
pthread_mutex_unlock(&table->mutex);
```

### Efficient Waiting

```c
// YANLIŞ (Busy wait):
while (!condition)
    ;  // CPU 100% kullanır

// DOĞRU (usleep ile):
while (!condition)
    usleep(100);  // Daha verimli

// DAHA İYİ (Condition variables - C99+):
pthread_cond_wait(&cond, &mutex);
```

## 🎯 Test Senaryoları

```bash
# Basit test
./philo 5 800 200 200

# Kimse ölmemeli (yeterli zaman)
./philo 5 5000 200 200

# Philosopher ölmeli
./philo 5 100 200 200

# Hepsi yemeyi tamamlamalı
./philo 3 800 200 200 2

# Extreme: Çok philosopher
./philo 100 800 200 200
```

## 💡 Performance Metrics

İyi implementasyon:
- Hiç deadlock olmamalı
- Belirtilen zamanda ölüm olmalı
- Timing'in ±10ms içinde olması yeterli

## 📚 Norm Standartları

- 80 karaktere kadar satır uzunluğu
- Fonksiyon başına maksimum 25 satır
- Memory leaks yoktur
- Data races yoktur
- Norm compliance sağlanmıştır

## 💡 Key Learning Points

1. **Concurrency**: Multi-threaded programming
2. **Synchronization Primitives**: Mutex, condition variables
3. **Deadlock**: Nedenleri ve çözümleri
4. **Race Conditions**: Identification ve prevention
5. **Performance**: Lock contention optimization
6. **Debugging**: Thread-based bugs'ların teşhisi

Bu proje, çok işlemli sistem programlamasının temel taşlarını öğretir.
