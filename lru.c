#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_FRAMES 100
#define MAX_REFERENCES 1000
#define MAX_SCENARIOS 1000
#define ALGORITHM "LRU"
#define OUTPUT_FILE "outputlru.txt"

static int find_page(const int frames[], int capacity, int page) {
    for (int i = 0; i < capacity; i++)
        if (frames[i] == page) return i;
    return -1;
}

static void print_frames(FILE *out, const int frames[], int capacity) {
    printf("[ ");
    fprintf(out, "[ ");
    for (int i = 0; i < capacity; i++) {
        if (frames[i] == -1) {
            printf("- ");
            fprintf(out, "- ");
        } else {
            printf("%d ", frames[i]);
            fprintf(out, "%d ", frames[i]);
        }
    }
    printf("]");
    fprintf(out, "]");
}

static int choose_victim(const int pages[], int n, int step,
                         const int frames[], int capacity,
                         const int last_used[], const int frequency[],
                         const int loaded_at[], int fifo_next) {
    int victim = 0;
    if (ALGORITHM[0] == 'F') return fifo_next;

    if (ALGORITHM[0] == 'L' && ALGORITHM[1] == 'R') {
        for (int i = 1; i < capacity; i++)
            if (last_used[i] < last_used[victim]) victim = i;
        return victim;
    }

    if (ALGORITHM[0] == 'L') {
        for (int i = 1; i < capacity; i++)
            if (frequency[i] < frequency[victim] ||
                (frequency[i] == frequency[victim] &&
                 loaded_at[i] < loaded_at[victim]))
                victim = i;
        return victim;
    }

    if (ALGORITHM[0] == 'O') {
        int farthest = -1;
        for (int i = 0; i < capacity; i++) {
            int next = n;
            for (int j = step + 1; j < n; j++) {
                if (pages[j] == frames[i]) {
                    next = j;
                    break;
                }
            }
            if (next > farthest) {
                farthest = next;
                victim = i;
            }
        }
        return victim;
    }
    return 0;
}

static void simulate(const int pages[], int n, int capacity,
                     FILE *out, int scenario) {
    int frames[MAX_FRAMES], last_used[MAX_FRAMES];
    int frequency[MAX_FRAMES], loaded_at[MAX_FRAMES];
    int hits = 0, faults = 0, fifo_next = 0;

    for (int i = 0; i < capacity; i++) {
        frames[i] = -1;
        last_used[i] = -1;
        frequency[i] = 0;
        loaded_at[i] = -1;
    }

    printf("\n>>> KỊCH BẢN %d <<<\n", scenario);
    fprintf(out, "\n>>> KỊCH BẢN %d <<<\n", scenario);
    printf("\n=== MÔ PHỎNG THUẬT TOÁN LRU (Frames = %d) ===\n", capacity);
    fprintf(out, "\n=== MÔ PHỎNG THUẬT TOÁN LRU (Frames = %d) ===\n", capacity);
    printf("Bước\tTrang\tTrạng thái RAM\tKết quả\tTrang bị thay\n");
    fprintf(out, "Bước\tTrang\tTrạng thái RAM\tKết quả\tTrang bị thay\n");
    printf("----------------------------------------------------------------\n");
    fprintf(out, "----------------------------------------------------------------\n");

    for (int step = 0; step < n; step++) {
        int page = pages[step];
        int position = find_page(frames, capacity, page);
        int removed = -1;
        int is_hit = position != -1;

        if (is_hit) {
            hits++;
            frequency[position]++;
        } else {
            faults++;
            for (int i = 0; i < capacity; i++) {
                if (frames[i] == -1) {
                    position = i;
                    break;
                }
            }
            if (position == -1) {
                position = choose_victim(pages, n, step, frames, capacity,
                                         last_used, frequency, loaded_at, fifo_next);
                removed = frames[position];
            }

            frames[position] = page;
            frequency[position] = 1;
            loaded_at[position] = step;
            if (ALGORITHM[0] == 'F') fifo_next = (fifo_next + 1) % capacity;
        }

        last_used[position] = step;

        printf("%d\t%d\t", step + 1, page);
        fprintf(out, "%d\t%d\t", step + 1, page);
        print_frames(out, frames, capacity);
        printf("\t%s\t", is_hit ? "HIT" : "FAULT");
        fprintf(out, "\t%s\t", is_hit ? "HIT" : "FAULT");
        if (removed == -1) {
            printf("-\n");
            fprintf(out, "-\n");
        } else {
            printf("%d\n", removed);
            fprintf(out, "%d\n", removed);
        }
    }

    double hit_rate = n ? (double)hits * 100.0 / n : 0.0;
    double fault_rate = n ? (double)faults * 100.0 / n : 0.0;
    printf("----------------------------------------------------------------\n");
    fprintf(out, "----------------------------------------------------------------\n");
    printf("Tổng lượt tham chiếu: %d\nSố lần HIT: %d\nSố lỗi trang (FAULT): %d\nTỷ lệ HIT: %.2f%%\nTỷ lệ FAULT: %.2f%%\n",
           n, hits, faults, hit_rate, fault_rate);
    fprintf(out, "Tổng lượt tham chiếu: %d\nSố lần HIT: %d\nSố lỗi trang (FAULT): %d\nTỷ lệ HIT: %.2f%%\nTỷ lệ FAULT: %.2f%%\n",
            n, hits, faults, hit_rate, fault_rate);
}

int main(void) {
    FILE *in = fopen("input.txt", "r");
    if (in == NULL) {
        fprintf(stderr, "Lỗi: Không thể mở file input.txt!\n");
        return EXIT_FAILURE;
    }

    FILE *out = fopen(OUTPUT_FILE, "w");
    if (out == NULL) {
        fprintf(stderr, "Lỗi: Không thể tạo file kết quả!\n");
        fclose(in);
        return EXIT_FAILURE;
    }

    int scenarios;
    if (fscanf(in, "%d", &scenarios) != 1 ||
        scenarios < 1 || scenarios > MAX_SCENARIOS) {
        fprintf(stderr, "Lỗi: Số kịch bản trong input.txt không hợp lệ!\n");
        fclose(in);
        fclose(out);
        return EXIT_FAILURE;
    }

    printf("=== BÁO CÁO THUẬT TOÁN LRU ===\n");
    fprintf(out, "=== BÁO CÁO THUẬT TOÁN LRU ===\n");

    for (int scenario = 1; scenario <= scenarios; scenario++) {
        int capacity, n;
        if (fscanf(in, "%d %d", &capacity, &n) != 2 ||
            capacity < 1 || capacity > MAX_FRAMES ||
            n < 0 || n > MAX_REFERENCES) {
            fprintf(stderr, "Lỗi: Dữ liệu kịch bản %d không hợp lệ!\n", scenario);
            fclose(in);
            fclose(out);
            return EXIT_FAILURE;
        }

        int pages[MAX_REFERENCES];
        for (int i = 0; i < n; i++) {
            if (fscanf(in, "%d", &pages[i]) != 1 || pages[i] < 0) {
                fprintf(stderr, "Lỗi: Chuỗi tham chiếu ở kịch bản %d không hợp lệ!\n", scenario);
                fclose(in);
                fclose(out);
                return EXIT_FAILURE;
            }
        }
        simulate(pages, n, capacity, out, scenario);
    }

    fclose(in);
    fclose(out);
    printf("\nĐã chạy xong. Kết quả được lưu trong file %s\n", OUTPUT_FILE);
    return EXIT_SUCCESS;
}

