# Mô phỏng các thuật toán thay thế trang

Chương trình mô phỏng bốn thuật toán thay thế trang trong quản lý bộ nhớ ảo: FIFO, LRU, LFU và OPT. Mỗi chương trình đọc cùng một tập kịch bản, in các bước xử lý và thống kê để so sánh số lỗi trang.

## Các thuật toán

- **FIFO:** thay trang được nạp vào bộ nhớ sớm nhất.
- **LRU:** thay trang có lần sử dụng gần nhất cách hiện tại lâu nhất.
- **LFU:** thay trang có tần suất sử dụng thấp nhất; nếu bằng nhau, thay trang được nạp sớm hơn. Tần suất được tính trong lần trang đang nằm trong bộ nhớ và được đặt lại khi trang được nạp lại.
- **OPT:** thay trang sẽ được sử dụng xa nhất trong tương lai, hoặc không còn được sử dụng. OPT biết trước chuỗi tham chiếu nên được dùng làm mốc so sánh lý tưởng.

## Các file

| Mã nguồn | File kết quả |
|---|---|
| `fifo.c` | `outputfifo.txt` |
| `lru.c` | `outputlru.txt` |
| `lfu.c` | `outputlfu.txt` |
| `opt.c` | `outputopt.txt` |

`input.txt` chứa dữ liệu đầu vào chung cho cả bốn chương trình.

## Biên dịch và chạy

Cần trình biên dịch C hỗ trợ chuẩn C11, ví dụ GCC. Mở terminal tại thư mục dự án và chạy:

```bash
gcc -std=c11 fifo.c -o fifo
gcc -std=c11 lru.c -o lru
gcc -std=c11 lfu.c -o lfu
gcc -std=c11 opt.c -o opt

./fifo
./lru
./lfu
./opt
```

Mỗi chương trình đọc `input.txt` từ thư mục đang chạy và ghi kết quả ra màn hình cùng file tương ứng. Chạy lại sẽ ghi đè file kết quả của thuật toán đó.

## Định dạng dữ liệu đầu vào

File `input.txt` có dạng:

```text
<số kịch bản>
<số khung trang> <số lượt tham chiếu>
<chuỗi trang tham chiếu>
...
```

Ví dụ có hai kịch bản:

```text
2
3 7
1 2 3 2 4 1 3
3 9
1 1 2 3 4 2 5 2 3
```

Kịch bản đầu có 3 khung trang và 7 lượt tham chiếu. Mỗi số trang là số nguyên không âm. Chương trình hỗ trợ từ 1 đến 1.000 kịch bản, từ 1 đến 100 khung trang và tối đa 1.000 lượt tham chiếu mỗi kịch bản.

## Đọc kết quả và so sánh

Mỗi bước cho biết trang được tham chiếu, trạng thái các khung trang, kết quả **HIT** hoặc **FAULT**, và trang bị thay nếu có. Dấu `-` biểu thị khung trống hoặc không có trang bị thay.

Phần cuối mỗi kịch bản thống kê tổng lượt tham chiếu, số lần HIT, số lỗi trang, tỷ lệ HIT và tỷ lệ FAULT. Khi so sánh các thuật toán trên cùng kịch bản, thuật toán có ít lỗi trang hơn sử dụng các khung trang hiệu quả hơn theo chỉ số này.

File đầu vào hiện có 6 kịch bản. Ba kịch bản đầu lần lượt cho FIFO, LRU và LFU có ít lỗi trang nhất trong ba thuật toán này; kịch bản 4 và 5 dùng cùng chuỗi với 3 và 4 khung để quan sát ảnh hưởng của số khung. OPT có thể bằng hoặc tốt hơn các thuật toán còn lại về số lỗi trang.
