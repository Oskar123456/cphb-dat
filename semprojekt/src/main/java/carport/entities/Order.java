package carport.entities;

public record Order (
                    int Id,
                    int UserId,
                    String CreateDate,
                    String DesiredDate,
                    String FinishDate,
                    int Bot,
                    int Top,
                    int Quant
) {}
