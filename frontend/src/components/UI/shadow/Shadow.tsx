import cls from "@/utils/cls"
import styles from "./Shadow.module.css"

interface ShadowProps {
    className?: string
    onClick?: () => void
}

export const Shadow = ({ className = "", onClick }: ShadowProps): JSX.Element => (
    <div className={cls(className, styles.shadow)} onClick={onClick}></div>
)
