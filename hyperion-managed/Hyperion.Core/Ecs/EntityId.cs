namespace Hyperion.Ecs {
  public readonly struct EntityId {
    public static EntityId Empty => new(0xFFFFFFFF);

    public long Value { get; }

    public int Index => (int)Value;
    public int Version => (int)(Value >> 32);

    public EntityId(long value) { Value = value; }
    
    public override bool Equals(object obj) => obj is EntityId other && Equals(other); 
    public override int GetHashCode() => Value.GetHashCode();
    public override string ToString() => Value.ToString();

    public static implicit operator long(EntityId id) => id.Value;
    public static implicit operator EntityId(long value) => new(value);
    
    public static bool operator ==(EntityId a, EntityId b) => a.Value == b.Value;
    public static bool operator !=(EntityId a, EntityId b) => a.Value == b.Value;
  }
}